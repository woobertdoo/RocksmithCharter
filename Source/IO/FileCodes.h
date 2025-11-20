#pragma once

#include <juce_core/juce_core.h>
#include <stdio.h>

enum GP_EXTRACT_STATUS {
    GP_ERR_UNZIP,
    GP_ERR_SCORE_OPEN,
    GP_ERR_SCORE_READ,
    GP_SUCCESS
};

class GPError {
  private:
    GP_EXTRACT_STATUS status;
    juce::String message;

  public:
    GPError(juce::String errmessage, GP_EXTRACT_STATUS errstatus) {
        this->message = errmessage;
        this->status = errstatus;
    }

    void printError() {
        char* errorStatus = (char*)malloc(1024);
        switch (status) {
        case GP_ERR_SCORE_OPEN:
            errorStatus = (char*)"GP_ERR_SCORE_OPEN";
            break;
        case GP_ERR_UNZIP:
            errorStatus = (char*)"GP_ERR_UNZIP";
            break;
        case GP_ERR_SCORE_READ:
            errorStatus = (char*)"GP_ERR_SCORE_READ";
            break;
        case GP_SUCCESS:
            errorStatus = (char*)"GP_SUCCESS";
            break;
        }
        printf("ERROR: %s %s\n", errorStatus, message.toRawUTF8());
    }
};
