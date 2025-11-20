#include "GPExtractor.h"
#include <cstdio>
#include <juce_core/juce_core.h>
#include <memory>

GPError
GPExtractor::ExtractScore(juce::String gpName,
                          std::unique_ptr<juce::XmlElement>& scoreRoot) {
    juce::FileInputStream gpStream = juce::FileInputStream(juce::File(gpName));

    printf("Opening file %s\n", gpName.toRawUTF8());

    if (gpStream.failedToOpen()) {
        // TODO: Log Failed To Open GP Error
        return GPError(juce::String("Error Unzipping File"),
                       GP_EXTRACT_STATUS::GP_ERR_UNZIP);
    }
    juce::ZipFile gpFile(&gpStream, false);

    printf("Locating score.gpif\n");

    const juce::ZipFile::ZipEntry* scoreEntry =
        gpFile.getEntry("Content/score.gpif");
    if (!scoreEntry) {
        // TODO: Log Failed To Load Score Error
        return GPError(juce::String("Error Loading Score"),
                       GP_EXTRACT_STATUS::GP_ERR_SCORE_OPEN);
    }

    printf("Loading score.gpif\n");

    std::unique_ptr<juce::InputStream> scoreStream(
        gpFile.createStreamForEntry(*scoreEntry));
    juce::String scoreString = scoreStream->readEntireStreamAsString();

    scoreRoot = juce::parseXMLIfTagMatches(scoreString, "GPIF");

    if (!scoreRoot) {
        // TODO: Log Failed To Read Score Error
        return GPError(juce::String("Error Reading Score"),
                       GP_EXTRACT_STATUS::GP_ERR_SCORE_READ);
    }

    printf("Done\n");

    return GPError(juce::String("Success!"), GP_EXTRACT_STATUS::GP_SUCCESS);
}
