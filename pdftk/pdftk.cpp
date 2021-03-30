
//
// pdftk, version 1.0
// PDF Type3 tests
//

#include "pdfwriter.h"

#include <cstdio>
#include <cstring>
#include <string>
#include <map>
#include <fstream>
#include <iostream>


static int
make_pdf( const char* pdf_filename, char* target )
{
    std::string pdf_fn(pdf_filename);

    std::ofstream fout;
    fout.open(pdf_fn, std::ios::out | std::ios::binary | std::ios::trunc);

    if (!fout.is_open()) {
        std::cerr << "error: open file for output failed!" << std::endl;
    }

    PdfWriter pdfWriter(&fout);

    pdfWriter.writeHeader();

    {
        pdfWriter.addType3Font();
        pdfWriter.pageContent();
    }
    {
     //   pdfWriter.pageContentHello();
    }
 
    pdfWriter.end();

  return 0;
}

static void version();
static void help();

int main(int argc, char *argv[])
{
    { // help? version?
        bool help_b = (argc != 2);
        bool version_b = false;
        for (int ii = 1; ii < argc; ++ii) {
            help_b = help_b || (strcmp(argv[ii], "--help") == 0);
            version_b = version_b || (strcmp(argv[ii], "--version") == 0);
        }
        if (help_b) {
            version();
            help();
            return 0;
        }
        else if (version_b) {
            version();
            return 0;
        }
    }

    return make_pdf(argv[1], 0);

}

static void version()
{
	std::cout << std::endl;
    std::cout << "pdftk 1.00" << std::endl;
	std::cout << std::endl;
}

static void help()
{
  std::cout << "Usage:" << std::endl;
  std::cout << "   pdftk <PDF path and filename>" << std::endl;
  std::cout << std::endl;
  std::cout << "Where:" << std::endl;
  std::cout << "   pdftoc takes a PDF file" << std::endl;

}

