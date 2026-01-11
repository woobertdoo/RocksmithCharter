/*
  ==============================================================================

    This file contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include <memory>

#include "Chart/Chart.h"
#include "IO/GPExtractor.h"
#include "IO/GPParser.h"

#include "GUI/MainComponent.h"

//==============================================================================
class MainWindowTutorialApplication : public juce::JUCEApplication {
  public:
    //==============================================================================
    MainWindowTutorialApplication() {}

    const juce::String getApplicationName() override {
        return JUCE_APPLICATION_NAME_STRING;
    }
    const juce::String getApplicationVersion() override {
        return JUCE_APPLICATION_VERSION_STRING;
    }
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
            printf("Setting Main Component\n");
            setContentOwned(new MainComponent(), true);
            printf("Finished Setting Main Component\n");
            setUsingNativeTitleBar(true);
            centreWithSize(getWidth(), getHeight());
            setVisible(true);
            //            loadGPFile();
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

            gpParser->PopulateTrackMap(gpScore);
            std::vector<std::unique_ptr<juce::XmlElement>> masterBars =
                gpParser->GetMasterBars("Drumkit");

            std::string trackName = masterBars[0]
                                        ->getChildByName("TrackName")
                                        ->getAllSubText()
                                        .toStdString();

            std::string bars = masterBars[0]
                                   ->getChildByName("Ids")
                                   ->getAllSubText()
                                   .toStdString();

            printf("Track %s uses bars %s !\n", trackName.c_str(),
                   bars.c_str());

            /*  XmlVector bars = gpParser->GetBars(masterBars.at(0));

              for (auto& bar : bars) {
                  int barId = bar->getIntAttribute("id");

                  printf("Bar %d has voice %d\n", barId,
                         bar->getChildByName("Voices")
                             ->getAllSubText()
                             .getIntValue());
              }
          */
        }

        void closeButtonPressed() override {
            juce::JUCEApplication::getInstance()->systemRequestedQuit();
        }

      private:
        std::unique_ptr<GPExtractor> gpExtractor;
        std::unique_ptr<GPParser> gpParser;
        MainComponent* mainComponent;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
    };

  private:
    std::unique_ptr<MainWindow> mainWindow;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION(MainWindowTutorialApplication)
