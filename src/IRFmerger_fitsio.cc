/* ======================================================================== *\                                      
!                                                                                                                            
!   Author(s): John E Ward, Javier Rico     12/2014 <mailto:jward@ifae.es>                                                           
!                                                                                                                               
!   Copyright: CTA Software Development, 2014                                                                           
!                                                                                                                                          
\* ======================================================================== */

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <iterator>
#include <iomanip>
#include <math.h>

//GIRF includes
#include "GIRF.h"

//FITS includes
#include "fitsio.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
//
// Declare functions here
//

void makeFITS(std::vector<float> pdf_data, string fitsname, 
	      const AxisType axis_type[],
	      const VarType var_type[],
	      const int axis_size[], const bool  axisislog[], 
	      float *axis[], const int naxes);

void checkFitConsistency(string fitFile, 
			 std::vector< std::vector<float> > axis_values, 
			 std::vector<int> axis_variables, 
			 std::vector<int> axis_coding,
			 string firstfFile,
                         std::vector< std::vector<float> > first_axisvals,
                         std::vector<int> first_valtypes,
                         std::vector<int> first_axistypes,
			 int filecount);

////////////////////////////////////////////////////////////////////////////////
//
// Main code starts here


int main(int argc, char **argv)
{


  if(argc != 3) //Do more checking on the number of arguments below                                                                                                
    {

      cerr << argv[0] << ": config and pdf-file not specified" << endl;

      cout << "Usage: " << argv[0]
	   << " configfile.txt PDFfiles.txt"
	   << endl;

      return -1;

    }

  ifstream config_file ( argv[1] );
  ifstream pdf_file ( argv[2] );

  //Check to see if file opening succeeded
  if( !config_file.is_open() )
    {

      cout<<"Could not open config file\n";     
      return -1;
      
    }
  else if ( !pdf_file.is_open() )
    {
      
      cout<<"Could not open PDF file\n";
      return -1;

    }

  string ifile_name = argv[2];
  string fit_file;

  fitsfile* pfit;

  char keyval[FLEN_VALUE];

  std::vector <float> axis1;
  float* paxis;

  std::vector< std::vector<float> > first_axisvals;
  std::vector<int> first_valtypes;
  std::vector<int> first_axistypes;
  string firstfFile;

  std::vector<int> new_vartype;
  std::vector<int> new_axistype;

  int filecount = 0;
  int hdupos    = 0;
  int hdunum    = 0;      
  int naxis_ent = 0; //Number of entries to axis object 
  int atype     = 0;
  int vtype     = 0;
  int status    = 0; //Initialize status 
  
  //Keywords
  const int numkey = 4;
  const string keyname[numkey]={ "NAXIS", "NAXIS1", "AXISTYPE", "VARTYPE" };
  
  cout<<"Reading Performance files from: "<<ifile_name<<endl;
  
  std::vector <float> bindata;
  float* pbindata;
  
  std::vector <float> fulldata;

  long fpixel[2]; 
  //Corresponds to starting cell 1,1
  fpixel[0] = 1; //NAXIS1, x
  fpixel[1] = 1; //NAXIS2, y
  
  while(pdf_file >> fit_file){
    
    //Consistency variables, reset for each file
    std::vector< std::vector<float> > axis_values; 
    std::vector <int> axis_variables; //Variable type storage vector
    std::vector <int> axis_coding; //Axis type storage vector
    string fitcurrent;
    
    cout<<endl;
    cout<<"Opening fit file ... "<<fit_file.c_str()<<endl;
    
    status = 0;  //Re-initialize status 
    
    //Use when a HDU name or number to open was not explicitly 
    //specified as part of the filename. Goes to first image HDU
    //with NAXIS greater than 0
    
    if (!fits_open_data(&pfit, fit_file.c_str(), READONLY, &status)){
      
      cout<<"File opened!"<<endl;
      cout<<"Getting Header Data Units (HDUs) ... "<<endl;
      fits_get_hdu_num(pfit, &hdupos);
      
      for (; !status; hdupos++){  //Main loop through each HDU
	
	for(int i=0; i<numkey; i++){
	  
	  if (fits_read_keyword(pfit, keyname[i].c_str(), keyval, NULL, &status)){	    
	    
	    //Keyword does not exist
	    keyval[0] = '\0';
	    status = 0;  //Reset status after error 
	    
	  }
	  else{
	    switch(i){
	    case 0:
	      
	      //If NAXIS>=2, we are in the datablock and should skip.
	      if(atoi(keyval)>=2){
		cout<<"Reached datablock - Move to next file"<<endl;
		i=numkey;
		//Before break, have the number of axis in this fits file
		break;
	      }
	      else{
		cout<<"Reading axis block ... "<<endl;
	      }
	      break;

	    case 1:

	      naxis_ent=atoi(keyval);	      
	      axis1.resize(naxis_ent);
	      paxis = axis1.data();
	      fits_read_pix(pfit, TFLOAT, fpixel, naxis_ent, NULL, paxis, NULL, &status);

	      cout<<"Axis #: "<<hdupos-1<<" Keyword "<<keyname[i]<<": "<<naxis_ent<<endl;
	      axis_values.push_back(axis1);
 	      break;	    

	    case 2:

	      atype=atoi(keyval);
	      cout<<"Axis #: "<<hdupos-1<<" Keyword "<<keyname[i]<<": "<<atype<<endl;
	      axis_coding.push_back(atype);
	      break;

	    case 3:

	      vtype=atoi(keyval);
	      cout<<"Axis #: "<<hdupos-1<<" Keyword "<<keyname[i]<<": "<<vtype<<endl;
	      axis_variables.push_back(vtype);
	      break;
	      
	    default:

	      cout<<"Error: No NAXIS1, AXISTYPE or VARTYPE found in FITS header!"<<endl;
	      exit(EXIT_FAILURE);

	    }//Ends switch(i)

	  }

	}//Ends i
	
	//Do consistency check between axes here
	//Need to compare the current axis1, atype and vtype with the previous
	
	fits_movrel_hdu(pfit, 1, NULL, &status);  //Move to next HDU
	
      }//Ends hdu loop
      
      if (status == END_OF_FILE)  status = 0; // Reset after normal error
      
      //Move to temp variables, all following files to be checked against the first       
      if(filecount==0){		

	firstfFile=fit_file.c_str();
	first_axisvals=axis_values;
	first_valtypes=axis_variables;
	first_axistypes=axis_coding;
	
      }

      checkFitConsistency(fit_file, axis_values, axis_variables, axis_coding, 
			  firstfFile, first_axisvals, first_valtypes, first_axistypes, 
			  filecount);

      int nelements=1;
      for(int i=0; i<(hdunum-1); i++){
	nelements*=first_axisvals[i].size();
      }

      bindata.resize(nelements);
      pbindata=bindata.data();

      fits_get_num_hdus(pfit, &hdunum, &status);
      hdunum=hdunum-1; //Remove HDU for data block
      long npixel[hdunum];

      //Corresponds to starting cell for ND 
      for(int i=0; i<hdunum; i++){
	npixel[i]=1;
      }
      
      if(fits_read_pix(pfit, TFLOAT, npixel, nelements, NULL, pbindata, NULL, &status)){  //Read pixels
	
	cout<<"Error reading pixels!"<<endl;
	cout<<"Exiting with status: "<<status<<endl;
	break;
	
      }
      
      cout<<endl;
      cout<<"Copying to full array"<<endl;
      cout<<" ... "<<endl;

      //Here we have the datacube array, this will fill nelements*number of files
      for(int i=0;i<nelements;i++){	
	fulldata.push_back(bindata.at(i));
      }
      
      cout<<"Closing fit file ... "<<fit_file<<endl;          
      fits_close_file(pfit, &status);
    
    }//Ends fits open file
    else{
      
      cout<<"Could not open!: "<<fit_file<<endl;
      cout<<"Does the file/directory exist?"<<endl;
      exit(EXIT_FAILURE);
      
    }
    
    filecount++;
  
  }//Ends while
  
  pdf_file.close();
  
  int axes_num  = hdunum-1; 
  int naxis_size = filecount;

  std::vector<int> axis_size;

  for(int i=0; i<axes_num; i++){
    axis_size.push_back(first_axisvals[i].size());
  }

  //Nth dimension
  axis_size.push_back(naxis_size);
 
  //Completely arbitrary Z-direction bins (number of files) for now
  float naxis_vals[] = {10,20,30,40,50,60,70,80,90,100, 
		       10,20,30,40,50,60,70,80,90,100,
		       10,20,30,40,50,60,70,80,90,100,
		       10,20,30,40,50,60,70,80,90,100,
		       10,20,30,40,50,60,70,80,90,100,
		       10,20,30,40,50,60,70,80,90,100,
		       10,20,30,40,50,60,70,80,90,100,
		       10,20,30,40,50,
		       10,20,30,40,50,60,70,80,90,100, 
		       10,20,30,40,50,60,70,80,90,100,
		       10,20,30,40,50,60,70,80,90,100,
		       10,20,30,40,50,60,70,80,90,100,
		       10,20,30,40,50,60,70,80,90,100,
		       10,20,30,40,50,60,70,80,90,100,
		       10,20,30,40,50,60,70,80,90,100,
		       10,20,30,40,50,
		       10,20,30,40,50,60,70,80,90,100,
		       10,20,30,40,50,60,70,80,90,100,
		       10,20,30,40,50,60,70,80,90,100,
		       10,20,30,40,50,60,70,80,90,100,
		       10,20,30,40,50,60,70,80,90,100,
		       10,20,30,40,50,60,70,80,90,100,
		       10,20,30,40,50,60,70,80,90,100,
		       10,20,30,40,50,60,70,80,90,100};

  std::vector<float*> axis;
  for(int i=0; i<axes_num; i++){
    axis.push_back(first_axisvals[i].data());
  }

  //Nth dimension
  axis.push_back(naxis_vals);

  //Histname will need to provide a sensible title
  string histname="datacube_test";

  /* Currently HDUs do not specify whether axes are log or not, 
     add to girf code so that this information can be parsed from 
     the input files 
  */
  const bool  axisislog[]  = {true, false, false, false};

  int new_axis_num=axes_num+1;
  
  //The new axis is Phi and parameterized 
  new_axistype=first_axistypes;
  new_axistype.push_back(kParam);
  new_vartype=first_valtypes;
  new_vartype.push_back(kPhi);

  AxisType axis_type[new_axis_num];
  VarType var_type[new_axis_num];

  for(int i=0; i<new_axis_num; i++){
    axis_type[i]=static_cast<AxisType>(new_axistype[i]);
  }
  for(int i=0; i<new_axis_num; i++){
    var_type[i]=static_cast<VarType>(new_vartype[i]);
  }
  
  makeFITS(fulldata, histname, axis_type, var_type, axis_size.data(), axisislog, axis.data(), new_axis_num);

  cout<<endl;Main
  cout<<"Merged "<<filecount<<" fits files into "<<histname<<".fits"<<endl;
  cout<<endl;
  
}//Ends main

void makeFITS(std::vector<float> pdf_data, string histname, 
	      const AxisType axis_type[], const VarType var_type[],
	      const int axis_size[], const bool  axisislog[], 
	      float *axis[], int naxes)
{
  
  //Declare and fill pdf                                                                                                           
  GIRFPdf*   mypdf   = new GIRFPdf(kEfficiency,kNumber);
  mypdf->SetData(pdf_data.data());

  //Declare and fill pdf                                                                                                           
  GIRFPdf*   mypdf2   = new GIRFPdf(kEDispersion,kGaussian);
  mypdf2->SetData(pdf_data.data());

  GIRFPdf*   mypdf3   = new GIRFPdf(kEDispersion,kGaussian);
  mypdf3->SetData(pdf_data.data());

  GIRFPdf*   mypdf4   = new GIRFPdf(kEDispersion,kGaussian);
  mypdf4->SetData(pdf_data.data());

  GIRFAxis** IRFAxis = new GIRFAxis*[naxes];
  
  //Fill the axes                                                                                                                      
  for(int iaxis=0;iaxis<naxes;iaxis++)
    {
      
      //Simpler notation                                                                         
      AxisType axistype = axis_type[iaxis];
      VarType  vartype  = var_type[iaxis];
      bool               islog    = axisislog[iaxis];
      int                size     = axis_size[iaxis];
      float*             theaxis  = axis[iaxis];
      
      //Fill the GIRF axis objects                                                                      
      switch(axistype)
        {
        case kBins:

          IRFAxis[iaxis] = new GIRFAxisBins(vartype,size,theaxis,islog);
          break;

        default:
        	//TODO: Currently not supporting parameterized axes.
//          IRFAxis[iaxis] = new GIRFAxisParam(vartype,size,theaxis,islog);
          break;

        }
      
      mypdf->AddAxis(IRFAxis[iaxis]);
      mypdf2->AddAxis(IRFAxis[iaxis]);
      mypdf3->AddAxis(IRFAxis[iaxis]);
      mypdf4->AddAxis(IRFAxis[iaxis]);

    }

  //Fill pdf (which includes axes already) in the GIRF object                                                              
  GIRF* irf = new GIRF;
  irf->AddPdf(mypdf);
  irf->AddPdf(mypdf2);
  irf->AddPdf(mypdf3);
  irf->AddPdf(mypdf4);

  //Write the IRF to file                                                                                            
  string fitsout="!"+histname+".fits";
  irf->Write(fitsout);
  
}//Ends makePDF

void checkFitConsistency(string fit_file,
                         std::vector< std::vector<float> > axis_values,
                         std::vector<int> axis_variables,
                         std::vector<int> axis_coding,
                         string firstfFile,
                         std::vector< std::vector<float> > first_axisvals,
                         std::vector<int> first_valtypes,
                         std::vector<int> first_axistypes,
			 int filecount){
  
  if(first_axisvals!=axis_values){
    
    cout<<endl;
    cout<<"        - Inconsistent Axes -         "<<endl;
    cout<<"       Bin Values not the same!       "<<endl;
    cout<<"   Please check the bin values below  "<<endl;
    cout<<endl;
    
    if(first_axisvals.size()==axis_values.size()){
      for ( std::vector< std::vector<int> >::size_type i = 0; i < axis_values.size(); i++ )
	{
	  cout<<"File #: "<<filecount
	      <<" Path : "<<firstfFile
	      <<" Axis #: "<<(i+1)<<setw(5)
	      <<" Number of Bins: "<<axis_values[i].size()
	      <<endl;
	  cout<<"With Lower Edge Bin Values ..."<<endl;
	  cout<<"File # 1 - File # "<<filecount<<endl;
	  
	  for ( std::vector<int>::size_type j = 0; j < axis_values[i].size(); j++ )
	    {
	      cout<<fixed<<setprecision(2)<<first_axisvals[i][j]<<setw(11)<<axis_values[i][j]<< endl;
	    }
	  cout << endl;
	}
      exit(EXIT_FAILURE);
    }
    else{
      cout<<" Number of Bins differ "<<endl;
    }
  }
  
  /*Consistency check between elements of
    first and current VARTYPE vectors*/
  if(first_valtypes!=axis_variables){
    
    cout<<endl;
    cout<<"        - Inconsistent Axes -         "<<endl;
    cout<<"Variable Types (VARTYPE) not the same!"<<endl;
    cout<<endl;
    
    unsigned int ind=0;
    while(ind != first_valtypes.size()){
      if(first_valtypes.at(ind)!=axis_variables.at(ind)){
	cout<<"File #: 1"<<endl;
	cout<<firstfFile<<" Axis #: "<<ind<<" has VARTYPE "<<first_valtypes.at(ind)<<endl;
	cout<<"different from "<<endl;
	cout<<"File #: "<<filecount<<endl;
	cout<<fit_file<<" Axis #: "<<ind<<" has VARTYPE "<<axis_variables.at(ind)<<endl;
      }
      ind++;
    }
    
    cout<<endl;
    exit(EXIT_FAILURE);
    
  }
  
  /*Consistency check between elements of
    first and current VARTYPE vectors*/
  if(first_axistypes!=axis_coding){
    
    cout<<endl;
    cout<<"        - Inconsistent Axes -         "<<endl;
    cout<<"Axis Types (AXISTYPE) not the same!"<<endl;
    cout<<endl;
    
    unsigned int ind=0;
    while(ind != first_axistypes.size()){
      if(first_axistypes.at(ind)!=axis_coding.at(ind)){
	cout<<"File #: 1"<<endl;
	cout<<firstfFile<<" Axis #: "<<ind<<" has AXISTYPE "<<first_axistypes.at(ind)<<endl;
	cout<<"different from "<<endl;
	cout<<"File #: "<<filecount<<endl;
	cout<<fit_file<<" Axis #: "<<ind<<" has AXISTYPE "<<axis_coding.at(ind)<<endl;
      }
      ind++;
    }
    
    cout<<endl;
    exit(EXIT_FAILURE);
    
  }
  
}//Ends checkFitConsistency
