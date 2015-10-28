/* ======================================================================== *\                             
!                                                                                                                                                   
!   author(s): John E Ward, Javier Rico     12/2014 <mailto:jward@ifae.es>                                                                        
!                                                                                                                                                      
!   Copyright: CTA Software Development, 2014                                                                                                              
!                                                                                                                                                     
\* ======================================================================== */

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <iterator>

#include <math.h>
#include "GIRF.h"

//ROOT includes
#include <rootincludes.h>

using namespace std;

////////////////////////////////////////////////////////////////////////////////
//
// Declare functions here
//

void irfGenND(TFile *f, string histname, 
	      const GIRFAxis::AxisType axistype[], 
	      const GIRFAxis::VarType vartype[],
	      const int naxes);
void makeFITS(vector <float> pdfdata, string fitsname, 
	      const GIRFAxis::AxisType axistype[], 
	      const GIRFAxis::VarType vartype[], 
	      const int axissize[], const bool  axisislog[], 
	      float *axis[], const int naxes);

////////////////////////////////////////////////////////////////////////////////
//
// Main code starts here
//

int main(int argc, char **argv)
{

  if(argc != 3) // do more checking on the number of arguments below                                                                                                
    {
      cerr << argv[0] << ": config and pdf-file not specified" << endl;

      cout << "Usage: " << argv[0]
	   << " configfile.txt PDFparams.root"
	   << endl;

      return -1;
    }

  ifstream configfile ( argv[1] );
  ifstream pdffile ( argv[2] );

  // Check to see if file opening succeeded
  if( !configfile.is_open() )
    {
      cout<<"Could not open config file\n";     
      return -1;
    }

  if ( !pdffile.is_open() )
    {
      cout<<"Could not open PDF file\n";
      return -1;
    }

  string input_filename = argv[2];
  cout<<"Reading "<<input_filename<<endl;

  /*
    Need to use the config file to read the following:

    PDF type: kNoPdfVar=0,kEfficiency,kEDispersion,kPsf,kBkgRate,kPdfVarMax
    PDF Para: kNoPdfFunc=0,kNumber,kGaussian,k2DGaussian,kPdfFuncMax
  */

  TFile *f = new TFile(input_filename.c_str());

  //Performance Histogram
  const int numest=5; //number of estimators 
  //  const int naxes[]={ 2, 2, 3 };
  //  string histname[]={ "ERes_offaxis", "Ebias_offaxis", "AngRes_offaxis" };
  const int naxes[]={ 1, 1, 1, 1, 2};
  string histname[]={ "BGRate", "DiffSens", "AngRes", "EffectiveAreaEtrue", "MigMatrix" };

  for(int i=0; i<numest; i++){
    
    if(!f->GetListOfKeys()->Contains(histname[i].c_str())){
      cout<<"Error! Performance info "<<histname[i]<<" is not provided. "<<endl;
      exit(EXIT_FAILURE);
    }
    
    GIRFAxis::AxisType axistype[naxes[i]];
    GIRFAxis::VarType vartype[naxes[i]];
    
    switch(i)
      {
      case 0: //Energy Resolution for E (kEnergy) v. Offset (kTheta)
	axistype[0]   = GIRFAxis::kBins;
	vartype[0]    = GIRFAxis::kEnergy;	
	irfGenND(f, histname[i], axistype, vartype, naxes[i]);
	break;
      case 1: //Energy Bias for E (kEnergy) v. Offset (kTheta)
	axistype[0]   = GIRFAxis::kBins;
	//	vartype[0]    = GIRFAxis::kEnergy, vartype[1] = GIRFAxis::kTheta;	
	vartype[0]    = GIRFAxis::kEnergy;	
	irfGenND(f, histname[i], axistype, vartype, naxes[i]);
	break;
      case 2: //Angular Resolution for E (kEnergy) v. Offset (kTheta) v. Azimuth (kPhi)
	axistype[0]   = GIRFAxis::kBins;
	vartype[0]    = GIRFAxis::kEnergy;	
	irfGenND(f, histname[i], axistype, vartype, naxes[i]);
	break;
      case 3: //Angular Resolution for E (kEnergy) v. Offset (kTheta) v. Azimuth (kPhi)
	axistype[0]   = GIRFAxis::kBins;
	vartype[0]    = GIRFAxis::kEnergy;
	irfGenND(f, histname[i], axistype, vartype, naxes[i]);
	break;
      case 4: //Angular Resolution for E (kEnergy) v. Offset (kTheta) v. Azimuth (kPhi)
	axistype[0]   = GIRFAxis::kBins, axistype[1] = GIRFAxis::kBins;
	vartype[0]    = GIRFAxis::kEnergy, vartype[1] = GIRFAxis::kTheta;	
	irfGenND(f, histname[i], axistype, vartype, naxes[i]);
	break;
      }
            
  }//ends i loop
  
}//ends main

void irfGenND(TFile *paramfile, string histname, const GIRFAxis::AxisType axistype[], const GIRFAxis::VarType vartype[], const int naxesArr)
{

  //Estimator histogram from analysis file
  //Beware this will need to change to some fits reader function
  TH2F* estim=(TH2F*)paramfile->Get(histname.c_str());
  
  //input or per parameter
  const int naxes = naxesArr; 

  //data vector
  vector <float> pdfdata;
  
  //declare the axes and link them to pdf                                                                           
  float* axis[naxesArr];

  int xaxis_size = 0;
  int yaxis_size = 0;
  int zaxis_size = 0;

  if(naxesArr==1){
    xaxis_size    = estim->GetXaxis()->GetNbins();
    yaxis_size    = 0;
    zaxis_size    = 0;
    pdfdata.reserve(xaxis_size);
  }
  else if(naxesArr==2){
    xaxis_size    = estim->GetXaxis()->GetNbins();
    yaxis_size    = estim->GetYaxis()->GetNbins();
    zaxis_size    = 0;
    pdfdata.reserve(xaxis_size*yaxis_size);
  }
  else if(naxesArr==3){
    xaxis_size    = estim->GetXaxis()->GetNbins();
    yaxis_size    = estim->GetYaxis()->GetNbins();
    zaxis_size    = estim->GetYaxis()->GetNbins(); //Making dummy Z-axis
    pdfdata.reserve(xaxis_size*yaxis_size*zaxis_size);
  }
  else{
    cout<<"Error when finding axes sizes: "<<endl;
    cout<<"NAxis out of bounds (0 or >3)"<<endl;
    exit(EXIT_FAILURE);
  }

  const int   axissize[]   = {xaxis_size, yaxis_size, zaxis_size};
  const bool  axisislog[]  = {true, false, false};
  
  // actual data: axes and pdf parameters                                             
  // Pull out bin width and loop to fill this array 
  float xaxis_vals[xaxis_size];
  float yaxis_vals[yaxis_size];
  float zaxis_vals[zaxis_size];

  cout<<" - Building fits file with the following dimensions - "<<endl;
  cout<<"X: "<<xaxis_size<<" Y: "<<yaxis_size<<" Z: "<<zaxis_size<<endl;
   
  for(int i=1;i<=xaxis_size;i++){
    xaxis_vals[i-1]=estim->GetXaxis()->GetBinLowEdge(i);
  }
  
  for(int j=1;j<=yaxis_size;j++){
    yaxis_vals[j-1]=estim->GetYaxis()->GetBinLowEdge(j);
  }

  for(int k=1;k<=zaxis_size;k++){
    zaxis_vals[k-1]=estim->GetYaxis()->GetBinLowEdge(k);
  }
    
  if(naxesArr==1){
      for(int i=1;i<=xaxis_size;i++){
	  
	pdfdata.push_back(estim->GetBinContent(i));
	
      }//ends i (x-axis)
  }

  else if(naxesArr==2){
      for(int j=1;j<=yaxis_size;j++){
	for(int i=1;i<=xaxis_size;i++){
	  
	pdfdata.push_back(estim->GetBinContent(i,j));
	
	}//ends i (x-axis)
      }//ends j (y-axis)
  }
  else if(naxesArr==3){
    for(int k=1;k<=zaxis_size;k++){
      for(int j=1;j<=yaxis_size;j++){
	for(int i=1;i<=xaxis_size;i++){
	  
	pdfdata.push_back(estim->GetBinContent(i,j));
	
	}//ends i (x-axis)
      }//ends j (y-axis)
    }//ends k (z-axis) 
  }
  else{
    cout<<"Error when filling pdf data: "<<endl;
    cout<<"NAxis out of bounds (0 or >3)"<<endl;
    exit(EXIT_FAILURE);
  }

  if(naxesArr==1){
    axis[0] = xaxis_vals;
    axis[1] = 0;
    axis[2] = 0;
  }
  else if(naxesArr==2){
    axis[0] = xaxis_vals;
    axis[1] = yaxis_vals;
    axis[2] = 0;
  }
  else if(naxesArr==3){
    axis[0] = xaxis_vals;
    axis[1] = yaxis_vals;
    axis[2] = zaxis_vals;
  }
  else{
    cout<<"Error when defining axes objects: "<<endl;
    cout<<"NAxis out of bounds (0 or >3)"<<endl;
    exit(EXIT_FAILURE);
  }

  makeFITS(pdfdata, histname, axistype, vartype, axissize, axisislog, axis, naxesArr);
  
}//end irfGenND

void makeFITS(vector <float> pdfdata, string histname, 
	      const GIRFAxis::AxisType axistype[], const GIRFAxis::VarType vartype[], 
	      const int axissize[], const bool  axisislog[], 
	      float *axis[], int naxes)
{
  
  // declare and fill pdf                                                                                                           
  GIRFPdf*   mypdf   = new GIRFPdf(GIRFPdf::kEfficiency,GIRFPdf::kNumber);
  mypdf->SetData(&pdfdata[0]);
  
  GIRFAxis** IRFAxis = new GIRFAxis*[naxes];

  // fill the axes                                                                                                                      
  for(int iaxis=0;iaxis<naxes;iaxis++)
    {
      
      // simpler notation                                                                         
      GIRFAxis::AxisType axis_type = axistype[iaxis];
      GIRFAxis::VarType  var_type  = vartype[iaxis];
      bool               islog    = axisislog[iaxis];
      int                size     = axissize[iaxis];
      float*             theaxis  = axis[iaxis];
      
      // fill the GIRF axis objects                                                                      
      switch(axis_type)
        {
        case GIRFAxis::kBins:
          IRFAxis[iaxis] = new GIRFAxisBins(var_type,size,theaxis,islog);
          break;
        default:
        	//TODO: Currently not supporting parameterized axes.
//          IRFAxis[iaxis] = new GIRFAxisParam(var_type,size,theaxis,islog);
          break;
        }
      
      mypdf->AddAxis(IRFAxis[iaxis]);
    }

  // Fill pdf (which includes axes already) in the GIRF object                                                              
  GIRF* irf = new GIRF;
  irf->AddPdf(mypdf);

  // Write the IRF to file                                                                                            
  string fitsout="!"+histname+".fits";
  irf->Write(fitsout);
  
}//ends makeFITS
