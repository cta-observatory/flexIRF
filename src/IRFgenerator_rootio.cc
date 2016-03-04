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
#ifdef WITH_ROOT  // ROOT found in the installation
	#include "rootincludes.h"
#endif

using namespace std;


#ifdef WITH_ROOT  // ROOT found in the installation


////////////////////////////////////////////////////////////////////////////////
//
// Declare functions here
//

void IRFgenerator_rootio_irfGenND(TFile *f, string histname,
	      const AxisType axistype[],
	      const VarType vartype[],
	      const int naxes);
void IRFgenerator_rootio_makeFITS(vector <float> pdfdata, string fitsname,
	      const AxisType axistype[],
	      const VarType vartype[],
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
    
    AxisType axistype[naxes[i]];
    VarType vartype[naxes[i]];
    
    switch(i)
      {
      case 0: //Energy Resolution for E (kEnergy) v. Offset (kTheta)
	axistype[0]   = kBins;
	vartype[0]    = kEnergy;
	IRFgenerator_rootio_irfGenND(f, histname[i], axistype, vartype, naxes[i]);
	break;
      case 1: //Energy Bias for E (kEnergy) v. Offset (kTheta)
	axistype[0]   = kBins;
	//	vartype[0]    = kEnergy, vartype[1] = kTheta;
	vartype[0]    = kEnergy;
	IRFgenerator_rootio_irfGenND(f, histname[i], axistype, vartype, naxes[i]);
	break;
      case 2: //Angular Resolution for E (kEnergy) v. Offset (kTheta) v. Azimuth (kPhi)
	axistype[0]   = kBins;
	vartype[0]    = kEnergy;
	IRFgenerator_rootio_irfGenND(f, histname[i], axistype, vartype, naxes[i]);
	break;
      case 3: //Angular Resolution for E (kEnergy) v. Offset (kTheta) v. Azimuth (kPhi)
	axistype[0]   = kBins;
	vartype[0]    = kEnergy;
	IRFgenerator_rootio_irfGenND(f, histname[i], axistype, vartype, naxes[i]);
	break;
      case 4: //Angular Resolution for E (kEnergy) v. Offset (kTheta) v. Azimuth (kPhi)
	axistype[0]   = kBins, axistype[1] = kBins;
	vartype[0]    = kEnergy, vartype[1] = kTheta;
	IRFgenerator_rootio_irfGenND(f, histname[i], axistype, vartype, naxes[i]);
	break;
      }
            
  }//ends i loop
  
}//ends main

void IRFgenerator_rootio_irfGenND(TFile *paramfile, string histname, const AxisType axistype[], const VarType vartype[], const int naxesArr)
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

  int xaxisBins = 0;
  int yaxisBins = 0;
  int zaxisBins = 0;

  if(naxesArr==1){
	  xaxisBins    = estim->GetXaxis()->GetNbins();
	  yaxisBins    = 0;
	  zaxisBins    = 0;
    pdfdata.reserve(xaxisBins);
  }
  else if(naxesArr==2){
	  xaxisBins    = estim->GetXaxis()->GetNbins();
	  yaxisBins    = estim->GetYaxis()->GetNbins();
	  zaxisBins    = 0;
    pdfdata.reserve(xaxisBins*yaxisBins);
  }
  else if(naxesArr==3){
	  xaxisBins    = estim->GetXaxis()->GetNbins();
	  yaxisBins    = estim->GetYaxis()->GetNbins();
	  zaxisBins    = estim->GetYaxis()->GetNbins(); //Making dummy Z-axis
    pdfdata.reserve(xaxisBins*yaxisBins*zaxisBins);
  }
  else{
    cout<<"Error when finding axes sizes: "<<endl;
    cout<<"NAxis out of bounds (0 or >3)"<<endl;
    exit(EXIT_FAILURE);
  }

  const int   axissize[]   = {xaxisBins+1, yaxisBins+1, zaxisBins+1};
  const bool  axisislog[]  = {true, false, false};
  
  // actual data: axes and pdf parameters                                             
  // Pull out bin width and loop to fill this array 
  float xaxis_vals[axissize[0]];
  float yaxis_vals[axissize[1]];
  float zaxis_vals[axissize[2]];

  cout<<" - Building fits file with the following dimensions - "<<endl;
  cout<<"X: "<<axissize[0]<<" Y: "<<axissize[1]<<" Z: "<<axissize[2]<<endl;
   
  for(int i=1;i<=axissize[0];i++){
    xaxis_vals[i-1]=estim->GetXaxis()->GetBinLowEdge(i);
  }
  
  for(int j=1;j<=axissize[1];j++){
    yaxis_vals[j-1]=estim->GetYaxis()->GetBinLowEdge(j);
  }

  for(int k=1;k<=axissize[2];k++){
    zaxis_vals[k-1]=estim->GetYaxis()->GetBinLowEdge(k);
  }
    
  if(naxesArr==1){
      for(int i=1;i<=axissize[0];i++){
	  
	pdfdata.push_back(estim->GetBinContent(i));
	
      }//ends i (x-axis)
  }

  else if(naxesArr==2){
      for(int j=1;j<=axissize[1];j++){
	for(int i=1;i<=axissize[0];i++){
	  
	pdfdata.push_back(estim->GetBinContent(i,j));
	
	}//ends i (x-axis)
      }//ends j (y-axis)
  }
  else if(naxesArr==3){
    for(int k=1;k<=axissize[2];k++){
      for(int j=1;j<=axissize[1];j++){
	for(int i=1;i<=axissize[0];i++){
	  
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

  IRFgenerator_rootio_makeFITS(pdfdata, histname, axistype, vartype, axissize, axisislog, axis, naxesArr);
  
}//end irfGenND

void IRFgenerator_rootio_makeFITS(vector <float> pdfdata, string histname,
	      const AxisType axistype[], const VarType vartype[],
	      const int axissize[], const bool  axisislog[], 
	      float *axis[], int naxes)
{
  
  // declare and fill pdf                                                                                                           
  GIRFPdf*   mypdf   = new GIRFPdf(kEfficiency,kNumber);
  mypdf->SetData(&pdfdata[0]);

  GIRFAxis** IRFAxis = new GIRFAxis*[naxes];

  // fill the axes                                                                                                                      
  for(int iaxis=0;iaxis<naxes;iaxis++)
    {
      
      // simpler notation                                                                         
      AxisType axis_type = axistype[iaxis];
      VarType  var_type  = vartype[iaxis];
      bool               islog    = axisislog[iaxis];
      int                size     = axissize[iaxis];
      float*             theaxis  = axis[iaxis];
      
      // fill the GIRF axis objects                                                                      
      switch(axis_type)
        {
        case kBins:
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
  string fitsout="!examples/"+histname+".fits";
  irf->Write(fitsout);

}//ends makeFITS

#endif


#ifndef WITH_ROOT  // ROOT found in the installation

int main(int argc, char **argv)
{
	cout << "This macro makes use of ROOT libraries (not found in your system during compilation)." << endl;
	cout << "Exitting..." << endl;
	return 0;
}

#endif
