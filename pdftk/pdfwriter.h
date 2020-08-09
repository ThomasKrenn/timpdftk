
#ifndef PDFWRITER_H
#define PDFWRITER_H

#include <vector>

class Postream;

class PdfWriter {  
public:

    PdfWriter(std::ostream* ostream);

    void writeHeader();
    void writePage();
    void writeFonts(int fontres);
    void pageContent();
    void pageContentHello();
    void addType3Font();
    void writeTail();
    void end();

protected:



    inline uint32_t requestObject() {

        return currentObject++;
    }

    void xprintf(const char* fmt, ...);
    void writeRawData(unsigned char* data, int size);

    int addXrefEntry(int object, bool printostr);

    void write(std::string &data);

    void writeXMP();
    int writeOutputIntents();
private:  

    void writePageRoot();

    std::vector<uint32_t> pages;

    size_t streampos;

    uint32_t currentObject;

    Postream* stream;

    std::vector<uint32_t> xrefPositions;

    // Catalog PDF objects
    int catalog, pageRoot, info;
    
    int type3FontID;
    int _pagecontent;
    int _xmp;
    int _outputIntents;
    std::vector<uint32_t>  fonts;
};

#endif
