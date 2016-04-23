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


#####THANKS/REFERENCES
Uses jdksmidi awesome library to read and parse midi files    
[https://github.com/jdkoftinoff/jdksmidi](https://github.com/jdkoftinoff/jdksmidi)

Thanks to [https://github.com/gmarco/qthesia](https://github.com/gmarco/qthesia) for example of parsing jdksmidi messages   
Nice writeup on setting up a virtual midi port on a Mac (works well with GarageBand)    
http://fox-gieg.com/tutorials/2007/inter-application-midi/

Uses RtMidi (included in ofxMidi) to send data to MIDI ports
[https://github.com/chrisoshea/ofxMidi](https://github.com/chrisoshea/ofxMidi)



