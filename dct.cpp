/*****************************copyright (c) 2016, Xilinx, Inc.
All rights reserved.
Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice, 
this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, 
this list of conditions and the following disclaimer in the documentation 
and/or other materials provided with the distribution.
3. Neither the name of the copyright holder nor the names of its contributors 
may be used to endorse or promote products derived from this software 
without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
************************************************/

/* ************ starting point of  module qfloatdct.c *************************** */
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <Magick++.h>
#include <cmath>

#include "dct.h"

using namespace Magick;
using namespace std;

double getPSNR(Image* src, Image* addwtm)
{
  Image image1 = *src;
  Image image2 = *addwtm;
  int w = image1.columns();
  int h = image1.rows();
  MagickCore::Quantum *pixels = image1.getPixels(0, 0, w, h);
  MagickCore::Quantum *pixels1 = image2.getPixels(0, 0, w, h);
  int row;
  int column;
  double tmp = 0;
  unsigned offset1;
  unsigned offset2;
  double absdiff;
  double MAX = 255;

  for(row = 0; row < h; row++)
  {
    for(column = 0; column < w; column++)
    {
      offset1 = image1.channels() * (w * row + column);
      offset2 = image2.channels() * (w * row + column);
      absdiff = abs(pixels[offset1 + 0]/65535*255 - pixels1[offset2 + 0]/65535*255);
      tmp = tmp + pow(absdiff, 2);                                                     
    }            
  }
  double MSE = tmp / (w*h);

  double PSNR = 10*log10(pow(MAX,2)/MSE);
  cout<< "PSNR = " << PSNR <<endl;

  return MSE;

  }

double getNC(Image* src, Image* addwtm)
{
  Image image1 = *src;
  Image image2 = *addwtm;
  int w = image1.columns();
  int h = image1.rows();
  MagickCore::Quantum *pixels = image1.getPixels(0, 0, w, h);
  MagickCore::Quantum *pixels1 = image2.getPixels(0, 0, w, h);
  int row;
  int column;
  double tmp = 0;
  unsigned offset1;
  unsigned offset2;
  double AA=0,AB=0,BB=0;


  for(row = 0; row < h; row++)
  {
    for(column = 0; column < w; column++)
    {
      offset1 = image1.channels() * (w * row + column);
      offset2 = image2.channels() * (w * row + column);
      AA = AA + pow(pixels[offset1 + 3]/65535*255, 2);
      BB = BB + pow(pixels1[offset2 + 0]/65535*255, 2);
      AB = AB + (pixels[offset1 + 3]/65535*255) * (pixels1[offset2 + 0]/65535*255);
    }
  }
  double NC = AB/(sqrt(AA)*sqrt(BB));
  cout << "NC = " << NC <<endl;
  return NC;

}



int main(void)
{

  InitializeMagick(NULL);
  Image src;
  Image addwtm;
  
  src.read("a01.png");
  //image.display();
  addwtm.read("image_w1a.png");
  //image1.display();
  //getPSNR(&src, &addwtm);
  
  getNC(&src, &addwtm);
  
  return 0;

}
  
  /* ***************** ending point of  module qfloatdct.c ************************* */

