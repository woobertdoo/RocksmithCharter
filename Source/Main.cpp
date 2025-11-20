/*
  ==============================================================================

    This file contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include <juce_core/juce_core.h>
#include <juce_core/system/juce_PlatformDefs.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <memory>

#include "Chart/Chart.h"
#include "IO/GPExtractor.h"
#include "IO/GPParser.h"

//==============================================================================
class MainWindowTutorialApplication : public juce::JUCEApplication {
  public:
    //==============================================================================
    MainWindowTutorialApplication() {}

    const juce::String getApplicationName() override {
        return "RocksmithCharter";
    }
    const juce::String getApplicationVersion() override { return "0.0.1"; }
    bool moreThanOneInstanceAllowed() override { return true; }

    //==============================================================================
    void initialise(const juce::String& commandLine) override {
        // Add your application's initialisation code here..
        mainWindow.reset(new MainWindow("Hello JUCE"));
        juce::String temp = commandLine;
    }

    void shutdown() override {
        // Add your application's shutdown code here..
        mainWindow = nullptr;
    }

    //==============================================================================
    void systemRequestedQuit() override {
        // This is called when the app is being asked to quit: you can ignore
        // this request and let the app carry on running, or call quit() to
        // allow the app to close.
        quit();
    }

    void anotherInstanceStarted(const juce::String& commandLine) override {
        // When another instance of the app is launched while this one is
        // running, this method is invoked, and the commandLine parameter tells
        // you what the other instance's command-line arguments were.
        juce::String temp = commandLine;
    }

    class MainWindow : public juce::DocumentWindow {
      public:
        MainWindow(juce::String name)
            : DocumentWindow(name, juce::Colours::lightgrey,
                             DocumentWindow::allButtons) {
            centreWithSize(300, 200);
            setUsingNativeTitleBar(true);
            setVisible(true);
            loadGPFile();
        }

        void loadGPFile() {
            std::unique_ptr<juce::XmlElement> gpScore;
            GPError gpExtractStatus = gpExtractor->ExtractScore(
                juce::String("/home/jamieharris/Documents/"
                             "RocksmithCharterCMAKE/test.gp"),
                gpScore);
            gpExtractStatus.printError();

            //            GPParser::FindTheWeirdElement(gpScore);

            gpParser = juce::rawToUniquePtr(new GPParser(gpScore));

            std::vector<std::unique_ptr<juce::XmlElement>> masterBars =
                gpParser->GetMasterBars();

            XmlVector bars = gpParser->GetBars(masterBars.at(0));

            for (auto& bar : bars) {
                int barId = bar->getIntAttribute("id");

                printf("Bar %d has voice %d\n", barId,
                       bar->getChildByName("Voices")
                           ->getAllSubText()
                           .getIntValue());
            }
        }

        void closeButtonPressed() override {
            juce::JUCEApplication::getInstance()->systemRequestedQuit();
        }

      private:
        std::unique_ptr<GPExtractor> gpExtractor;
        std::unique_ptr<GPParser> gpParser;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
    };

  private:
    std::unique_ptr<MainWindow> mainWindow;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION(MainWindowTutorialApplication)
