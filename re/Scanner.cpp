#include <sstream>

#include "Scanner.h"

using std::ostringstream;

Scanner::Scanner(istream & stream, ScannerInfo && scannerInfo)
    :inputStream(stream),
    transformTable(std::move(scannerInfo.transfromTable)),
    acceptTable(std::move(scannerInfo.acceptTable)),
    acceptStateToTokenMap(std::move(scannerInfo.acceptStateToTokenMap))
{
}

CodeToken Scanner::read()
{
    if (finish) {
        // why return an EOF token instead of throw a exception?
        // to keep similar behaviour as istream
        return{ CodeTokenType::Eof , "", currentRow };
    }

    int state = 1;
    int lastState = 0;
    
    ostringstream tokenStream;
    char ch;

    while (true) {
        ch = inputStream.peek();
        if (ch == Scanner::Eof) {
            finish = true;
            lastState = state;
            state = 0;
        } else {
            lastState = state;
            state = (*transformTable)[lastState][ch];
        }

        if (state == 0) {
            break;
        } else {
            tokenStream << ch;
            inputStream.get();
        }
    }

    if ((*acceptTable)[lastState] == true) {
        CodeTokenType tokenType = (*acceptStateToTokenMap)[lastState];

        if (tokenType == CodeTokenType::LineBreaker) {
            currentRow++;
        }

        if (skipTokens.find(tokenType) != skipTokens.end()) {
            // read next
            return read();
        } else {
            return{ tokenType, tokenStream.str(), currentRow };
        }
    } else {
        if (ch == Scanner::Eof) {
            return{ CodeTokenType::Eof, tokenStream.str(), currentRow };
        } else {
            return{ CodeTokenType::Unknown, tokenStream.str(), currentRow };
        }
    }

    //while (state != 0) {
    //    ch = inputStream.peek();
    //    if (ch == Scanner::Eof) {
    //        finish = true;
    //        lastState = state;
    //        state = 0;
    //    } else {
    //        lastState = state;
    //        state = (*transformTable)[lastState][ch];
    //        if (state != 0) {
    //            stream << ch;
    //            inputStream.get();
    //        }
    //    }
    //}

    //if ((*acceptTable)[lastState] == true) {
    //    CodeTokenType tokenType = (*acceptStateToTokenMap)[lastState];

    //    if (skipTokens.find(tokenType) != skipTokens.end()) {
    //        // read next
    //        if (finish) {
    //            return{ CodeTokenType::Eof, stream.str() };
    //        } else {
    //            return read();
    //        }
    //    } else {
    //        return{ tokenType, stream.str() };
    //    }
    //} else {
    //    if (ch == Scanner::Eof) {
    //        return{ CodeTokenType::Eof, stream.str() };
    //    } else {
    //        return{ CodeTokenType::Unknown, stream.str() };
    //    }
    //}
}

