/* ======================================================================== *\
!
!   Author(s): Javier Rico     12/2013 <mailto:jrico@ifae.es>
!
!   Copyright: CTA Software Development, 2013
!
\* ======================================================================== */
//////////////////////////////////////////////////////////////////////////////
//
// GIRFPdf
//
// Class to represent data describing a PDF of the IRF
//
//////////////////////////////////////////////////////////////////////////////


#include "GIRFPdf.h"
#include <iostream>

using namespace std;


////////////////////////////////////////////////////////////////
// 
// Construct pdf table with type
//
GIRFPdf::GIRFPdf(PdfVar pdftype,PdfFunc pdffunc,std::vector<GIRFAxis*>::size_type naxes) :
  fPdfVar(pdftype)
{
  fAxis.reserve(naxes);
}


////////////////////////////////////////////////////////////////
// 
// Write the pdf and the associated axes to the specified 
// file pointer
//
int GIRFPdf::Write(fitsfile* fptr,int ipdf,int& iaxis,int* status)
{
   // create arrays with size and first entry of every dimension to be saved (1 is first, not 0)
  int     naxis  = int(fAxis.size());
  long*   naxes  = new long[naxis];
  long*   fpixel = new long[naxis];
  for(std::vector<GIRFAxis*>::size_type jaxis=0;jaxis<naxis;jaxis++)
    {
      naxes[jaxis]  = int(fAxis[jaxis]->GetSize());
      fpixel[jaxis] = 1;
    }


  // write associated axis blocks
  int firstaxis = iaxis;
  for(vector<GIRFAxis*>::iterator axis = fAxis.begin(); axis!=fAxis.end(); ++axis)
    if((*axis)->Write(fptr,iaxis,status))
      cout << "GIRFPdf::Write Error: cannot write axis (error code: " << *status <<")" << endl;
  
  // write the pdf header
  if(fits_create_img(fptr,FLOAT_IMG,naxis,naxes,status))
    cout << "GIRFPdf::Write Error: problem writing axis header (error code: " << *status <<")" << endl;
    
  // write keywords to the header
  char keyword[9];
  char chval[20];
  char comment[70];
  ushort usval;
  
  // write the indeces linking to the relevant axes
  for(int jaxis=0;jaxis<naxis;jaxis++)
    {
      sprintf(keyword,"AXISID%d",jaxis+1);
      usval = ushort(jaxis+firstaxis);
      sprintf(comment,"Axis describing dimension #%d",jaxis+1);
      if(fits_write_key(fptr,TUSHORT,keyword,&usval,comment,status))
	cout << "GIRFPdf::Write Error: cannot write keyword (error code: " << *status <<")" << endl;
    }  

  // write pdf name
  sprintf(keyword,"EXTNAME");
  sprintf(chval,"DATA%03d",ipdf);
  sprintf(comment,"Pdf Data HDU");
  if(fits_write_key(fptr,TSTRING,keyword,&chval,comment,status))
    cout << "GIRFPdf::Write Error: cannot write keyword (error code: " << *status <<")" << endl;

  // write pdf var
  sprintf(keyword,"PDFVAR");
  usval = ushort(fPdfVar);
  sprintf(comment,"Variable whose pdf is parameterized (see GIRFPdf.h for details)");
  if(fits_write_key(fptr,TUSHORT,keyword,&usval,comment,status))
    cout << "GIRFPdf::Write Error: cannot write keyword (error code: " << *status <<")" << endl;
   
  // write pdf function
  sprintf(keyword,"PDFFunc");
  usval = ushort(fPdfFunc);
  sprintf(comment,"Function describing pdf (see GIRFPdf.h for details)");
  if(fits_write_key(fptr,TUSHORT,keyword,&usval,comment,status))
    cout << "GIRFPdf::Write Error: cannot write keyword (error code: " << *status <<")" << endl;
  
  // write the pdf data
  long nentries = GetSize();
  if(fits_write_pix(fptr,TFLOAT,fpixel,nentries,fData,status))
    cout << "GIRFPdf::Write Error: problem writing axis data (error code: " << *status <<")" << endl;

  return *status;
}
