#include "constants.h"

#include "GPParser.h"
#include "Note.h"
#include "NoteBuilder.h"
#include <bits/stdc++.h>
#include <hash_map>
#include <memory>
#include <sstream>

/*
 * <GPIF>
 *   -- Song Info (Title, etc.)
 *   <Score>
 *      <Title></Title>
 *      <Artist></Artist>
 *      <Album></Album>
 *   </Score>
 *   -- Track Info (name of track)
 *   -- Instrument Info (tuning, number of strings, etc)
 *   <Tracks>
 *     <Track id="x">
 *      <Name><![CDATA[trackname]]></Name>
 *      <Staves>
 *          <Staff>
 *              <Properties>
 *                  <Property name="CapoFret">
 *                      <Fret>x</Fret>
 *                  </Property>
 *                  <Property name="Tuning">
 *                      <!-- Pitches start at 0 = G1 -->
 *                  </Property>
 *              </Properties>
 *          </Staff>
 *      </Staves>
 *     </Track>
 *     .
 *     .
 *     .
 *   </Tracks>
 *   <MasterBars>
 *      -- Each master bar refers to ONE BAR.
 *      -- Each bar in the <Bars> tag is for a different TRACK (guitar 1, guitar
 * 2, bass, etc.).
 *      -- If the file has some sort of piano/keyboard, there will be two
 * different BAR TAGS for that track, one for the right hand and one for the
 * left hand.
 *      -- Get bar id offset by counting staves of each track?
 *
 *       <MasterBar>
 *          <Time>n/d</Time>
 *          <Repeat start="true/false" end="true/false" count=x/>
 *          <Bars>a b c ... </Bars>
 *          <!-- Which alternate ending of a repeat this uses -->
 *          <AlternateEndings>x</AlternateEndings>
 *      </MasterBar>
 *      .
 *      .
 *      .
 *   </MasterBars>
 *   <Bars>
 *      <Bar id="x">
 *          <!-- Only first voice is important -->
 *          <Voices> n -1 -1 -1 </Voices>
 *      </Bar>
 *      .
 *      .
 *      .
 *   </Bars>
 *
 *   <Voices>
 *      <Voice id="x">
 *      <!-- Note: Beats can repeat (0 1 2 1 2 3, etc.) -->
 *          <Beats>a b c d ... </Beats>
 *      </Voice>
 *   </Voices>
 *
 *  <Beats>
 *      <Beat id="x">
 *          <Rhythm ref="x" />
 *          <Notes>a b c ...</Notes>
 *          <!-- Unsure about arpeggio support atm -->
 *          <Arpeggio>Down/Up</Arpeggio>
 *      </Beat>
 *  </Beats>
 *
 *  <Notes>
 *      <Note id="x">
 *
 *          <Tie origin="true/false" destination="true/false"/>
 *
 *          <Property name="Fret">
 *              <Fret>x</Fret>
 *          </Property>
 *
 *          <!-- Slide Flags
 *
 *          1 = Shift (No LinkNext)
 *          2 = Legato (LinkNext)
 *          4 = Slide Out Down
 *          8 = Slide Out Up
 *          16 = Slide In From Below
 *          32 = Slide In From Above
 *
 *          -->
 *          <Property name="Slide">
 *          <Flags>x</Flags
 *          </Property>
 *
 *          <!-- Don't know if we care about the Enable tags -->
 *
 *          Enable Tag Properties:
 *          HopoOrigin
 *          HopoDestination
 *          LeftHandTapped
 *          Tapped
 *
 *          <Property name=*prop*>
 *              <Enable />
 *          </Property>
 *
 *          <Property name="String">
 *              <String>x</String>
 *          </Property>
 *      </Note>
 *  </Notes>
 *
 *  <Rhythms>
 *      <Rhythm id="x">
 *          <NoteValue>Whole/Half/Quarter/Eighth/16th/...</NoteValue>
 *          <PrimaryTuplet num="n" den="d"/>
 *          <AugmentationDot count="x"/>
 *      </Rhythm>
 *  </Rhythms>
 * </GPIF>
 *
 *
 * */

juce::String GPParser::GetGPVersion() {
    // The value of the node is considered a child
    const juce::XmlElement *versionNode = rootNode->getChildByName("GPVersion")->getFirstChildElement();
    return versionNode->getText();
}

void GPParser::SetRoot(std::unique_ptr<juce::XmlElement> &root) { *rootNode = *root; }

void GPParser::PopulateTrackMap(std::unique_ptr<juce::XmlElement> &root) {
    const juce::XmlElement *tracksNode = rootNode->getChildByName("Tracks");

    int barOffset = 0;

    for (auto track : tracksNode->getChildIterator()) {
        const juce::XmlElement *trackName = track->getChildByName("Name")->getFirstChildElement();
        if (trackName == nullptr) {
            printf("NO TRACK!\n");
            return;
        }
        std::string trackNameStr = "";
        if (trackName->isTextElement()) {
            trackNameStr = trackName->getText().toStdString();
        } else {
            printf("ERROR: This track has no name\n");
        }

        const juce::XmlElement *staves = track->getChildByName("Staves");
        int staffCount = staves->getNumChildElements();
        barOffsets[trackNameStr] = barOffset;
        barOffset += staffCount;
    }

    for (auto offs : barOffsets) {
        printf("%s has offset %d\n", offs.first.c_str(), offs.second);
    }
}

XmlVector GPParser::GetMasterBars(std::string trackName) {
    XmlVector trackBars;
    const juce::XmlElement *masterBarsNode = rootNode->getChildByName("MasterBars");
    const juce::XmlElement *firstMasterBar = masterBarsNode->getChildByName("MasterBar");
    const juce::XmlElement *firstBarSet = firstMasterBar->getChildByName("Bars");

    masterBarLength = firstBarSet->getAllSubText().getTrailingIntValue() + 1;

    std::vector<int> trackBarIds;
    std::string ids;

    for (auto masterBar : masterBarsNode->getChildIterator()) {

        std::vector<int> barIds = splitIds(masterBar->getChildByName("Bars")->getAllSubText().toStdString());

        const int trackBarOffset = barOffsets[trackName];

        for (int id : barIds) {
            if (id % masterBarLength != trackBarOffset) {
                continue;
            }
            ids.append(std::to_string(id));
            ids.append(" ");
            trackBarIds.push_back(id);
        }
    }
    // Remove trailing space from ids string
    ids.pop_back();
    juce::XmlElement *trackBarNode = new juce::XmlElement("TrackBars");
    juce::XmlElement *trackNameNode = new juce::XmlElement("TrackName");
    trackNameNode->addTextElement(juce::String(trackName));
    juce::XmlElement *idsNode = new juce::XmlElement("Ids");
    idsNode->addTextElement(juce::String(ids));

    trackBarNode->addChildElement(trackNameNode);
    trackBarNode->addChildElement(idsNode);

    trackBars.push_back(juce::rawToUniquePtr(trackBarNode));
    return trackBars;
}

XmlVector GPParser::GetBars(std::vector<int> barIds) {
    XmlVector bars;
    const juce::XmlElement *barsNode = rootNode->getChildByName("Bars");

    for (auto id : barIds) {
        const juce::XmlElement *foundBar = barsNode->getChildByAttribute("id", juce::String(id));
        if (!barsNode) {
            printf("Couldn't find bar with id %d\n", id);
            continue;
        }

        const juce::XmlElement *foundVoices = foundBar->getChildByName("Voices");

        if (!foundVoices) {
            printf("Something went very wrong, this bar (%d) has no voices!\n", id);
            continue;
        }

        juce::XmlElement *truncBar = new juce::XmlElement("Bar");
        juce::XmlElement *voices = new juce::XmlElement("Voices");
        juce::String firstVoiceId = foundVoices->getAllSubText().upToFirstOccurrenceOf(" ", false, false);
        voices->addTextElement(firstVoiceId);
        truncBar->setAttribute("id", id);
        truncBar->addChildElement(voices);
        bars.push_back(juce::rawToUniquePtr(truncBar));
    }

    return bars;
}

XmlVector GPParser::GetVoices(std::vector<int> voiceIds) {
    XmlVector voices;

    const juce::XmlElement *voicesNode = rootNode->getChildByName("Voices");

    for (auto id : voiceIds) {
        if (id == -1)
            continue;
        const juce::XmlElement *foundVoice = voicesNode->getChildByAttribute("id", juce::String(id));
        if (!voicesNode) {
            printf("Couldn't find voice with id %d\n", id);
            continue;
        }

        const juce::XmlElement *foundBeats = foundVoice->getChildByName("Beats");

        if (!foundBeats) {
            printf("Something went very wrong, this voice (%d) has no beats!\n", id);
            continue;
        }
    }

    return voices;
}

std::vector<Note *> GPParser::ParseNotes(int beatId) {
    std::vector<Note *> notes;

    const juce::XmlElement *beatsNode = rootNode->getChildByName("Beats");

    const juce::XmlElement *beatNode = beatsNode->getChildByAttribute("id", juce::String(beatId));

    if (!beatNode) {
        printf("No beat found with ID %d\n", beatId);
        return notes;
    }

    int rhythmId = beatNode->getChildByName("Rhythm")->getIntAttribute("ref", -1);

    const juce::XmlElement *rhythmsNode = rootNode->getChildByName("Rhythms");

    const juce::XmlElement *rhythmNode = rhythmsNode->getChildByAttribute("id", juce::String(rhythmId));

    if (!rhythmNode) {
        printf("No rhythm found with ID %d\n", beatId);
        return notes;
    }

    Rhythm rhythm;
    juce::String noteValue = rhythmNode->getChildByName("NoteValue")->getAllSubText();
    rhythm.baseVal = NoteValueToFlag(noteValue);

    if (juce::XmlElement *tuplet = rhythmNode->getChildByName("PrimaryTuplet")) {
        int num = tuplet->getIntAttribute("num");
        int den = tuplet->getIntAttribute("den");
        rhythm.tuple = Tuple {num, den};
    }

    if (juce::XmlElement *dots = rhythmNode->getChildByName("AugmentationDot")) {
        rhythm.dots = dots->getIntAttribute("count");
    }

    std::vector<int> noteIds = splitIds(beatNode->getChildByName("Notes")->getAllSubText().toStdString());

    juce::XmlElement *notesNode = rootNode->getChildByName("Notes");

    int tupleNotesProcessed = 0;
    int tieTickLength = 0;

    for (int id : noteIds) {
        const juce::XmlElement *foundNote = notesNode->getChildByAttribute("id", juce::String(id));
        if (!notesNode) {
            printf("Couldn't find note with id %d\n", id);
            continue;
        }
        if (tieTickLength == 0) {
            nb.NewNote();
        }
        const juce::XmlElement *noteProperties = foundNote->getChildByName("Properties");

        int fretNum =
            noteProperties->getChildByAttribute("name", "Fret")->getFirstChildElement()->getAllSubText().getIntValue();

        int stringNum = noteProperties->getChildByAttribute("name", "String")
                            ->getFirstChildElement()
                            ->getAllSubText()
                            .getIntValue();

        int tickLength = getTickLength(rhythm, tupleNotesProcessed);

        if (noteProperties->getChildByAttribute("name", "HopoDestination")) {
            // Sets current note flag to HOPO_DEST
            // Sets previous note flag to HOPO_SRC
            nb.ToggleHOPO(notes.back());
        }

        nb.SetString(stringNum);
        nb.SetFret(fretNum);

        if (juce::XmlElement *tie = foundNote->getChildByName("Tie")) {
            if (tie->getBoolAttribute("origin")) {
                tieTickLength += getTickLength(rhythm, tupleNotesProcessed);
                continue;
            } else if (tie->getBoolAttribute("destination")) {
                tickLength += getTickLength(rhythm, tupleNotesProcessed);
                tieTickLength = 0;
            }
        }

        nb.SetLengthInTicks(tickLength);

        if (juce::XmlElement *slide = noteProperties->getChildByAttribute("name", "Slide")) {
            int flagNum = slide->getChildByName("Flags")->getAllSubText().getIntValue();

            nb.ToggleSlide();
            if (flagNum == SlideFlags::LEGATO_SLIDE)
                nb.ToggleLinkNext();

            // TODO: Create new note 1/2 duration for slide out
            // Only if slide out is 1/4+ note long
            if (flagNum == SlideFlags::SLIDEOUT_DOWN || flagNum == SlideFlags::SLIDEOUT_UP) {
                if (tickLength >= CHART_TICKS_PER_QUARTER) {
                    nb.ToggleLinkNext();
                    nb.SetLengthInTicks(tickLength / 2);
                    notes.push_back(new Note(nb.BuildNote())); // Add original note
                    nb.NewNote();
                    nb.SetFret(fretNum);
                    nb.SetString(stringNum);
                    nb.SetRhythm({RhythmFlag(rhythm.baseVal * 2), rhythm.dots, rhythm.tuple});
                    nb.ToggleUnpitchedSlide(flagNum == SlideFlags::SLIDEOUT_DOWN ? fretNum - 1 : fretNum + 1);
                }
            }

            // TODO: Create new note for slide-ins
            // Only if slide in is 1/4+ note long
            if (flagNum == SlideFlags::SLIDEIN_BELOW || flagNum == SlideFlags::SLIDEIN_ABOVE) {
                if (rhythm.baseVal <= 4) {
                    Note noteStore = nb.BuildNote();
                }
            }
        }
        notes.push_back(new Note(nb.BuildNote()));
    }

    return notes;
}

RhythmFlag GPParser::NoteValueToFlag(juce::String noteValue) {
    if (noteValue.equalsIgnoreCase("Whole")) {
        return RHYTHM_WHOLE;
    } else if (noteValue.equalsIgnoreCase("Half")) {
        return RHYTHM_HALF;
    } else if (noteValue.equalsIgnoreCase("Quarter")) {
        return RHYTHM_QUARTER;
    } else if (noteValue.equalsIgnoreCase("Eighth")) {
        return RHYTHM_EIGHTH;
    } else if (noteValue.equalsIgnoreCase("16th")) {
        return RHYTHM_SIXTEENTH;
    } else if (noteValue.equalsIgnoreCase("32nd")) {
        return RHYTHM_THIRTYSECOND;
    } else {
        printf("No rhythm flag exists for %s, please add it!\n", noteValue.toStdString().c_str());
        return RHYTHM_INVALID;
    }
}

unsigned int GPParser::getNumSixteenths(const Rhythm &inRhythm) {
    if (inRhythm.baseVal >= 16)
        return 1;

    int numSixteenths = (16 / inRhythm.baseVal);
    for (int i = 1; i <= inRhythm.dots; i++) {
        int dotVal = inRhythm.baseVal * (2 ^ i);
        numSixteenths += 16 / dotVal;
    }

    return numSixteenths;
}

/*
 * Returns the amount of ticks a note should take up
 *
 *
 */

unsigned int GPParser::getTickLength(const Rhythm &inRhythm, int &tupleNotesProcessed) {
    const Tuple blank = {1, 1};

    // TODO: Implement check for minimum length for sustain

    /* quarter note = 16 ticks
     * eighth note = 8 ticks
     * 16th note = 4 ticks
     *
     * dotted quarter note = 24 ticks
     * quarter note triplet = floor((8 ticks * 2/3))
     */

    float numTicks = float(CHART_TICKS_PER_QUARTER * 4) / inRhythm.baseVal;
    if (inRhythm.dots > 0) {
        for (int i = 1; i <= inRhythm.dots; i++) {
            int dotVal = inRhythm.baseVal * (2 ^ i);
            numTicks += float(CHART_TICKS_PER_QUARTER * 4) / dotVal;
        }
    }

    // Default tuples are 1:1, so no check needed
    int noteSpace = inRhythm.tuple.den;
    float tickSpace = numTicks * noteSpace;
    int numNotes = inRhythm.tuple.num;
    numTicks *= tickSpace / numNotes;
    numTicks = std::floor(numTicks);

    tupleNotesProcessed += 1;
    if (tupleNotesProcessed < numNotes)
        return numTicks;

    int usedTicks = numTicks * tupleNotesProcessed;

    if (usedTicks < tickSpace) {
        numTicks += (tickSpace - usedTicks);
    }

    tupleNotesProcessed = 0;

    return numTicks;
}

void GPParser::FindTheWeirdElement() {
    const juce::XmlElement *masterBarsNode = rootNode->getChildByName("MasterBars");

    int id = 1;
    for (auto child : masterBarsNode->getChildIterator()) {
        for (auto subchild : child->getChildIterator()) {
            if (subchild->getChildByName("Bars") != nullptr) {
                printf("Weird node: %s parenting bars in masterbar %d\n", subchild->getTagName().toStdString().c_str(),
                       id);
            }
        }
        id++;
    }
    delete masterBarsNode;
}

/* For splitting children IDs to search for them
 *
 * i.e. splits a node <Nodes>1 2 3 4</Nodes>
 * into a vector {1, 2, 3, 4} to search for <Node id="1"> etc..
 * */
std::vector<int> GPParser::splitIds(std::string ids) {
    std::vector<int> splitVector;
    std::stringstream ss(ids);

    std::string t;

    char del = ' ';

    while (getline(ss, t, del)) {
        int t_int = std::stoi(t);
        splitVector.push_back(t_int);
    }

    return splitVector;
}
