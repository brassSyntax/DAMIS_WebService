#include "gtest/gtest.h"
#include "../Preprocess.h"
#include "../InitDamisService.h"
#include "../logging/easylogging++.h"
#include "../ServiceSettings.h"

#include <iostream>

_INITIALIZE_EASYLOGGINGPP

namespace {

class PreprocessTests : public testing::Test {

protected:

    virtual void SetUp() {
        easyloggingpp::Configurations conf(ServiceSettings::logingConfFilePath);

        InitDamisService *dFile = new InitDamisService("file:///home/damis/Documents/arff_files/random_1K.arff", "_input_");
        Preprocess *dRun = new Preprocess (dFile);
    }

};

    TEST_F(PreprocessTests, TestTest) {
        EXPECT_TRUE(true);
    }

}
