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
void IRFconverter_root2FITS_irfGenND(TFile *f, GIRF* irf, vector<float>& pdfdata, string histname,
	      const AxisType axistype[],
	      const VarType vartype[],
	      const PdfVar pdfvar_type,
	      const PdfFunc pdffunc_type,
	      const int naxes);

////////////////////////////////////////////////////////////////////////////////
//
// Main code starts here
//


#ifndef WITH_ROOT  // ROOT found in the installation

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

  TFile *fin = new TFile(input_filename.c_str());

  if (!fin->IsOpen()) {
    cout<<"Error: Cannot open input file "<<input_filename.c_str()<<endl;
    exit(EXIT_FAILURE) ;
  }
  
  TIter nextkey(fin->GetListOfKeys());
  int num_hists=fin->GetListOfKeys()->GetSize();
  TKey *key;
  TObject* obj ;                                                                                                                                                 

  const int numest=num_hists;
  int naxes[num_hists];
  string *histname = new string[num_hists];
  int num_objects=0;

  while (key = (TKey*)nextkey()) {
    obj = key->ReadObj() ;
    //Dont want to count anything that isn't a THist for the moment 
    //(although numest will have all objects counted)
    if (    (strcmp(obj->IsA()->GetName(),"TProfile")!=0)
	    && (!obj->InheritsFrom("TH1"))
	    && (!obj->InheritsFrom("TH2"))
	    && (!obj->InheritsFrom("TH3"))
	    ) 
      {
	
	cout<<"Warning: Object "<<obj->GetName()
	  <<" is not 1D, 2D or 3D histogram and will not be converted"<<endl;    
      }    

    //Fill arrays with dimensions and hist names
    if(obj->InheritsFrom("TH1"))
      {
	naxes[num_objects]=1;
	histname[num_objects]=obj->GetName();	
      }
    if(obj->InheritsFrom("TH2"))
      {	
    	naxes[num_objects]=2;
	histname[num_objects]=obj->GetName();	
      }
    if(obj->InheritsFrom("TH3"))
      {
   	naxes[num_objects]=3;//TH3
	histname[num_objects]=obj->GetName();     
      }
    num_objects++;
  }

  for(int i=0;i<numest;i++){
    cout<<"Hist: "<<histname[i]<<" Axis #: "<<naxes[i]<<endl;
  }
  
  vector< vector<float> > pdfgroup(numest);

  GIRF* irf = new GIRF;  
  
  //Need to use the config file to read the following:
  //PDF type: kNoPdfVar=0,kEfficiency,kEDispersion,kPsf,kBkgRate,kPdfVarMax
  //PDF Para: kNoPdfFunc=0,kNumber,kGaussian,k2DGaussian,kPdfFuncMax

  PdfVar pdfvar_type;
  PdfFunc pdffunc_type;

  for(int i=0; i<numest; i++){
    
    AxisType axistype[naxes[i]];
    VarType vartype[naxes[i]];
  
    //These switch statements are matched to the histogram objects 
    //located in the performance file Performance_2Q_Aar_50h_20150217.root
    //Need to think of either supplying this information in a config file, or via alternative means.

    switch(i)
      {
      case 0: //DiffSens
	axistype[0]   = kBins;
	vartype[0]    = kEnergy_rec;
	pdfvar_type   = kDiffSens;
	pdffunc_type  = kNumber;

	IRFconverter_root2FITS_irfGenND(fin, irf, pdfgroup[i], histname[i], axistype,
		 vartype, pdfvar_type, pdffunc_type, naxes[i]);
	break;
      case 1: //BGRate
	axistype[0]   = kBins;
	vartype[0]    = kEnergy;
	pdfvar_type   = kBkgRate;
	pdffunc_type  = kNumber;

	IRFconverter_root2FITS_irfGenND(fin, irf, pdfgroup[i], histname[i], axistype,
		 vartype, pdfvar_type, pdffunc_type, naxes[i]);
	break;
      case 2: //BGRatePerSqDeg
	axistype[0]   = kBins;
	vartype[0]    = kEnergy;
	pdfvar_type   = kBkgRateSqDeg;
	pdffunc_type  = kNumber;

	IRFconverter_root2FITS_irfGenND(fin, irf, pdfgroup[i], histname[i], axistype,
		 vartype, pdfvar_type, pdffunc_type, naxes[i]);
	break;
      case 3: //AngRes
	axistype[0]   = kBins, axistype[1] = kBins;
	vartype[0]    = kEnergy, vartype[1] = kEnergy;
	pdfvar_type   = kPsf;
	pdffunc_type  = kNumber;

	IRFconverter_root2FITS_irfGenND(fin, irf, pdfgroup[i], histname[i], axistype,
		 vartype, pdfvar_type, pdffunc_type, naxes[i]);
	break;
      case 4: //EffectiveAreaETrue
	axistype[0]   = kBins;
	vartype[0]    = kEnergy;
	pdfvar_type   = kAeff;
	pdffunc_type  = kNumber;

	IRFconverter_root2FITS_irfGenND(fin, irf, pdfgroup[i], histname[i], axistype,
		 vartype, pdfvar_type, pdffunc_type, naxes[i]);
	break;
      case 5: //EffectiveAreaEtrueNoTheta2cut
	axistype[0]   = kBins;
	vartype[0]    = kEnergy;
	pdfvar_type   = kAeffNoTheta2Cut;
	pdffunc_type  = kNumber;

	IRFconverter_root2FITS_irfGenND(fin, irf, pdfgroup[i], histname[i], axistype,
		 vartype, pdfvar_type, pdffunc_type, naxes[i]);
	break;
      case 6: //MigMatrix
	axistype[0]   = kBins; axistype[1] = kBins;
	vartype[0]    = kEnergy_rec; vartype[1] = kEnergy_true;
	pdfvar_type   = kEDispersion;
	pdffunc_type  = kNumber;

	IRFconverter_root2FITS_irfGenND(fin, irf, pdfgroup[i], histname[i], axistype,
		 vartype, pdfvar_type, pdffunc_type, naxes[i]);
	break;
      default:
	break;
      }
            
  }//ends i loop

  //Clean up
  delete irf;
  delete [] histname;

  cout<<"Finished conversion. "<<endl;
  cout<<endl;

  return 0;

}//ends main

void IRFconverter_root2FITS_irfGenND(TFile *paramfile, GIRF* irf,
	      vector<float>& pdfdata, string histname, 
	      const AxisType axistype[], const VarType vartype[],
	      const PdfVar pdfvar_type, const PdfFunc pdffunc_type,
	      const int naxes_array)
{

  //Estimator histogram from analysis file
  //Beware this will need to change to some fits reader function
  TCanvas *mydummycanvas=new TCanvas(); 
  TH2F* estim=(TH2F*)paramfile->Get(histname.c_str());

  if(estim){
    cout<<endl;
    cout<<"Accessing "<<histname.c_str()<<" from performance file. "<<endl;
    cout<<endl;
  }
  else{
    cout<<"Error: "<<histname.c_str()<<" not contained in performance file"<<endl;
    exit(EXIT_FAILURE);
  }

  //input or per parameter
  const int naxes = naxes_array; 
  
  //declare the axes and link them to pdf                                                                           
  float* axis[naxes_array];

  int xaxisBins = 0;
  int yaxisBins = 0;
  int zaxisBins = 0;

  if(naxes_array==1){
	  xaxisBins    = estim->GetXaxis()->GetNbins();
	  yaxisBins    = 0;
	  zaxisBins    = 0;
    pdfdata.reserve(xaxisBins);
  }
  else if(naxes_array==2){
	  xaxisBins    = estim->GetXaxis()->GetNbins();
	  yaxisBins    = estim->GetYaxis()->GetNbins();
	  zaxisBins    = 0;
    pdfdata.reserve((xaxisBins)*(yaxisBins));
  }
  else if(naxes_array==3){
	  xaxisBins    = estim->GetXaxis()->GetNbins();
	  yaxisBins    = estim->GetYaxis()->GetNbins();
	  zaxisBins    = estim->GetYaxis()->GetNbins(); //Making dummy Z-axis
    pdfdata.reserve((xaxisBins)*(yaxisBins)*(zaxisBins));
  }
  else{
    cout<<"Error when finding axes sizes: "<<endl;
    cout<<"NAxis out of bounds (0 or >3)"<<endl;
    exit(EXIT_FAILURE);
  }

  const int   axissize[]   = {xaxisBins+1, yaxisBins+1, zaxisBins+1};
  const bool  axisislog[]  = {true, false, false};
  
  //Actual data: axes and pdf parameters                                             
  //Pull out bin width and loop to fill this array 
  float xaxis_vals[axissize[0]];
  float yaxis_vals[axissize[1]];
  float zaxis_vals[axissize[2]];

  cout<<" - Building fits file with the following dimensions - "<<endl;
  cout<<" - X: "<<axissize[0]<<endl;
  cout<<" - Y: "<<axissize[1]<<endl;
  cout<<" - Z: "<<axissize[2]<<endl;
   
  for(int i=1;i<=axissize[0];i++){
    xaxis_vals[i-1]=estim->GetXaxis()->GetBinLowEdge(i);
  }
  
  for(int j=1;j<=axissize[1];j++){
    yaxis_vals[j-1]=estim->GetYaxis()->GetBinLowEdge(j);
  }

  for(int k=1;k<=axissize[2];k++){
    zaxis_vals[k-1]=estim->GetYaxis()->GetBinLowEdge(k);
  }
    
  if(naxes_array==1){
    for(int i=1;i<axissize[0];i++){
      
      pdfdata.push_back(estim->GetBinContent(i));
      
    }//ends i (x-axis)
  }
  
  else if(naxes_array==2){
    for(int j=1;j<axissize[1];j++){
      for(int i=1;i<axissize[0];i++){
	
	pdfdata.push_back(estim->GetBinContent(i,j));
	
      }//ends i (x-axis)
    }//ends j (y-axis)
  }
  else if(naxes_array==3){
    for(int k=1;k<axissize[2];k++){
      for(int j=1;j<axissize[1];j++){
    	for(int i=1;i<axissize[0];i++){
	  
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
  
  if(naxes_array==1){
    axis[0] = xaxis_vals;
    axis[1] = 0;
    axis[2] = 0;
  }
  else if(naxes_array==2){
    axis[0] = xaxis_vals;
    axis[1] = yaxis_vals;
    axis[2] = 0;
  }
  else if(naxes_array==3){
    axis[0] = xaxis_vals;
    axis[1] = yaxis_vals;
    axis[2] = zaxis_vals;
  }
  else{
    cout<<"Error when defining axes objects: "<<endl;
    cout<<"NAxis out of bounds (0 or >3)"<<endl;
    exit(EXIT_FAILURE);
  }
  
  //Declare and fill pdf                                                                                                           
  GIRFPdf*   mypdf   = new GIRFPdf(pdfvar_type, pdffunc_type);
  mypdf->SetData(pdfdata.data());
  
  GIRFAxis** IRFAxis = new GIRFAxis*[naxes];

  //Fill the axes                                                                                                                      
  for(int iaxis=0;iaxis<naxes;iaxis++)
    {
      
      //Simpler notation                                                                         
      AxisType axis_type = axistype[iaxis];
      VarType  var_type  = vartype[iaxis];
      bool               islog    = axisislog[iaxis];
      int                size     = axissize[iaxis];
      float*             theaxis  = axis[iaxis];
      
      //Fill the GIRF axis objects                                                                      
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

  //Fill pdf (which includes axes already) in the GIRF object                                                              
  irf->AddPdf(mypdf);

  // Write the IRF to file                                                                                            
  string fullfile="examples/testRoot2FITS_converter";
  string fitsout="!"+fullfile+".fits";

  cout<<endl;
  cout<<"Writing FITS file ..."<<endl;
  irf->Write(fitsout);
  
}//ends irfGenND
#endif


#ifndef WITH_ROOT  // ROOT found in the installation

int main(int argc, char **argv)
{
	cout << "This macro makes use of ROOT libraries (not found in your system during compilation)." << endl;
	cout << "Exitting..." << endl;
	return 0;
}

#endif


