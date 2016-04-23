#ofxThreadedMidiPlayer
Plays a midi file from disk in openFrameworks 0.9.3.

The example project has everything setup for XCode.

###What's new?
####Updated by [Roy J. Macdonald](https://github.com/roymacdonald/ofxThreadedMidiPlayer/)
* Updated to work with OF 0.9.3 (current stable release)
* Removed dependency on ofxRuiThread and now works with OF's ofThread object.
* Added ofEvent for each new midi event. This means you can easily set up any listener at any point in your app.
 This event uses the ofxMidiMessage form ofxMidi so it is very easy to deal with both addons together.
* Updated example.
* Restructured folder to conform with the ofxAddonTemplate
* Renamed README to README.md and updated it (this file)
* Rename testApp to ofApp
* added addon_config.mk file so it works nicely with OF's Project Generator



######Originally developed by [Jason van Cleave](https://github.com/jvcleave/ofxThreadedMidiPlayer/)
####Original Readme
#####THANKS/REFERENCES
Uses jdksmidi awesome library to read and parse midi files
[https://github.com/jdkoftinoff/jdksmidi](https://github.com/jdkoftinoff/jdksmidi)

Uses RtMidi (included in ofxMidi) to send data to MIDI ports
[https://github.com/chrisoshea/ofxMidi](https://github.com/chrisoshea/ofxMidi)

Thanks to [https://github.com/gmarco/qthesia](https://github.com/gmarco/qthesia) for example of parsing jdksmidi messages

Threading currently uses ofxRuiThread modified slightly to work with OF 007

NOTES/TO-DOS
iOS users - please see issue 1: https://github.com/jvcleave/ofxThreadedMidiPlayer/issues/1
ofxRuiThread probably is not needed but has made threading much simpler for me
Incorporation of ofxMidi?
Has not been tested with more than 1 port 
Nice writeup on setting up a virtual midi port on a Mac (works well with GarageBand)
http://fox-gieg.com/tutorials/2007/inter-application-midi/

