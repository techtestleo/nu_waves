/*
 * GuiApp.cpp
 *
 *  Created on: Oct 28, 2014
 *      Author: arturo
 */

#include "GuiApp.h"

int fbob=60;



// GUI Names
string AMOUNT="Amount";
string VELOCITY="Velocity";
string THRESHOLD="Threshold";
string NONE="None";
string BLEND="Blend";
string KEY="Key";
string DELAY="Delay";
string HUE="Hue";
string SAT="Sat";
string BRIGHT="Bright";
string H_INVERT="H invert";
string S_INVERT="S invert";
string B_INVERT="B invert";
string H_MOD="H mod";
string H_OFFSET="H offset";
string H_LFO="H lfo";
string X_DISPLACE="x displace";
string Y_DISPLACE="y displace";
string Z_DISPLACE="z displace";
string ROTATE="Rotate";
string SYPHON_OUT="syphon out";
string TETRA="tetrahedron";
string TOGGLE="Toggle";
string QUANTIZE="quantisation";
string MIX="mix";
string B_SCALE="bright scale";
string H_MIRROR="H Mirror";
string V_MIRROR="V Mirror";
string WRAP_SAT="Wrap Sat";
string WRAP_BRIGHT="Wrap Bright";
string H_POW_MAP="H pow map";
string S_POW_MAP="s pow map";
string B_POW_MAP="b pow map";
string H_POW_AMOUNT="H pow amount";
string S_POW_AMOUNT="S pow amount";
string B_POW_AMOUNT="B pow amount";
string CAMERA="Camera ";
string SYPHONER="Syphoner";
string CH1="CH1 ";
string B_0="B0 ";
string B_1="B1 ";
string XY="X / Y";
// midi
float c1=1;
float c2=1;
float c3=1;
float c4=1;
float c5=1;
float c6=1;
float c7=1;
float c8=1;
//
// int guiscale=100;
int guiwidth=275;

// Pad variables
// Modify the bounds to control the maximum amount of displacement.
int x_displace_bound = 3;
int y_displace_bound = 3;
// pad_W 200 gives 100 points of control either side of zero.
int pad_W = 200;
int half_width = pad_W/2;

void GuiApp::setup(){
    // midi
        // print input ports to console
        midiIn.listInPorts();
        
        // open port by number (you may need to change this)
        midiIn.openPort(00);
    //    midiIn.openPort("From MPKmini2");    // by name
        //midiIn.openVirtualPort("ofxMidiIn Input"); // open a virtual port
        
        // don't ignore sysex, timing, & active sense messages,
        // these are ignored by default
        midiIn.ignoreTypes(false, false, false);
        
        // add ofApp as a listener
        midiIn.addListener(this);
        
        // print received messages to the console
        midiIn.setVerbose(true);
    
    ofBackground(0);
    
    guisignal = new ofxDatGui( ofxDatGuiAnchor::TOP_LEFT );
    guisignal->setWidth(guiwidth);
    guisignal->addFRM();
    guisignal->addBreak();
    
    guisignal->addTextInput("nu_waves", "# v0.1 #");
    vector<string> channel1options ={CH1+NONE,CH1+CAMERA,CH1+SYPHONER};
  
 
    guisignal->addDropdown("CH1 IN", channel1options);
   
  
    vector<string> FBmixoptions ={B_0+NONE,B_0+BLEND,B_0+KEY};
    vector<string> FB1mixoptions ={B_1+NONE,B_1+BLEND,B_1+KEY};

    ///Buffer 0
    guisignal->addDropdown("Buffer 0", FBmixoptions);
    ofxDatGuiFolder* fb0mix_thingsfolder = guisignal->addFolder("Buffer 0 Mix", ofColor::white);
    
    fb0blendslider=fb0mix_thingsfolder->addSlider(AMOUNT,-5.0,5.0);
    fb0brightkeyamountslider=fb0mix_thingsfolder->addSlider(VELOCITY,0.00,1.00);
    fb0brightkeythreshslider=fb0mix_thingsfolder->addSlider(THRESHOLD,0.00,1.00);
    
    
    
    fb0brightkeyamountslider->bind(fb0lumakeyvalue);
    fb0brightkeythreshslider->bind(fb0lumakeythresh);
    fb0delayamountslider=fb0mix_thingsfolder->addSlider(DELAY,0,fbob-1);
    
    fb0blendslider->bind(fb0blend);
  
    fb0delayamountslider->bind(fb0delayamnt);
    
    fb0_hflip_toggle=fb0mix_thingsfolder->addToggle(B_0+H_MIRROR,false);
    fb0_vflip_toggle=fb0mix_thingsfolder->addToggle(B_0+V_MIRROR,false);
    

    ///FB1mixcontrol
    guisignal->addDropdown("Buffer 1", FB1mixoptions);
    ofxDatGuiFolder* fb1mix_thingsfolder = guisignal->addFolder("Buffer 1 Mix", ofColor::white);
    
    fb1blendslider=fb1mix_thingsfolder->addSlider(AMOUNT,-5.0,5.0);
    fb1brightkeyamountslider=fb1mix_thingsfolder->addSlider(VELOCITY,0.00,1.00);
    fb1brightkeythreshslider=fb1mix_thingsfolder->addSlider(THRESHOLD,0.00,1.00);
    
    fb1brightkeyamountslider->bind(fb1lumakeyvalue);
    fb1brightkeythreshslider->bind(fb1lumakeythresh);
    
    fb1blendslider->bind(fb1blend);
    
    fb1delayamountslider=fb1mix_thingsfolder->addSlider(DELAY,0,fbob-1);
    
    fb1delayamountslider->bind(fb1delayamnt);
    
    fb1_hflip_toggle=fb1mix_thingsfolder->addToggle(B_1+H_MIRROR,false);
    fb1_vflip_toggle=fb1mix_thingsfolder->addToggle(B_1+V_MIRROR,false);

    blur_amount_slider=guisignal->addSlider("Blur",0.0,5.0);
    blur_amount_slider->bind(blur_amount);
    
    sharpen_amount_slider=guisignal->addSlider("Sharpen",0.0,.3);
    sharpen_amount_slider->bind(sharpen_amount);

    
    cam1_scale_slider=guisignal->addSlider("Camera Scale",0,2);
    cam1_scale_slider->bind(cam1_scale);
    
    syphon_scale_slider=guisignal->addSlider("syphon scale",0,2);
    syphon_scale_slider->bind(syphon_scale);
  
    
    
    cam1_hflip_toggle=guisignal->addToggle(CAMERA+H_MIRROR,false);
    cam1_vflip_toggle=guisignal->addToggle(CAMERA+V_MIRROR,false);
    

    syphonOutputtoggle=guisignal->addToggle(SYPHON_OUT,false);
    tetrahedron_switch_toggle=guisignal->addToggle(TETRA,false);
    guisignal->onDropdownEvent(this, &GuiApp::onDropdownEvent);
    //guisignal->onColorPickerEvent(this, &GuiApp::onColorPickerEvent);
    
    //registertolistentoevents
    guisignal->onButtonEvent(this, &GuiApp::onButtonEvent);
    // x/y pad
    guisignal->addBreak();
    //
    
    
    
    
    ///-----------------------------------
    guithings = new ofxDatGui(guiwidth,0 );//ofxDatGuiAnchor::TOP_RIGHT );
    guithings->setWidth(guiwidth);
    guithings->addBreak();

    //channel1 hsb manipulation strip
    ofxDatGuiFolder* channel1_thingsfolder = guithings->addFolder("Channel HSB", ofColor::white);
    
    channel1hueslider= channel1_thingsfolder->addSlider("Hue", -5.0, 5.0);
    channel1saturationslider= channel1_thingsfolder->addSlider("Saturation", -5.0, 5.0);
    channel1brightslider= channel1_thingsfolder->addSlider("Brightness", -5.0, 5.0);
    

   
    channel1satwraptoggle=channel1_thingsfolder->addToggle(CH1+WRAP_SAT,false);
    channel1brightwraptoggle=channel1_thingsfolder->addToggle(CH1+WRAP_BRIGHT,false);
    
    channel1huepowmaptoggle=channel1_thingsfolder->addToggle(CH1+H_POW_MAP,false);
    channel1satpowmaptoggle=channel1_thingsfolder->addToggle(CH1+S_POW_MAP,false);
    channel1brightpowmaptoggle=channel1_thingsfolder->addToggle(CH1+B_POW_MAP,false);
    
    channel1huepowmapslider= channel1_thingsfolder->addSlider(H_POW_AMOUNT, -4.0, 4.0);
    channel1saturationpowmapslider= channel1_thingsfolder->addSlider(S_POW_AMOUNT, -4.0, 4.0);
    channel1brightpowmapslider= channel1_thingsfolder->addSlider(B_POW_AMOUNT, -4.0, 4.0);
    
  
   // channel1_thingsfolder->expand();

    //bind the gui channel1
    channel1hueslider->bind(channel1hue);
    channel1saturationslider->bind(channel1saturation);
    channel1brightslider->bind(channel1bright);
    
    channel1huepowmapslider->bind(channel1huepowmap);
    channel1saturationpowmapslider->bind(channel1saturationpowmap);
    channel1brightpowmapslider->bind(channel1brightpowmap);
    

    //fb0
    ofxDatGuiFolder* fb0_opsfolder = guithings->addFolder("Buffer 0 ops", ofColor::white);
    fb0_hue_slider= fb0_opsfolder->addSlider(HUE, 0.0, 20.0);
    fb0_saturation_slider= fb0_opsfolder->addSlider(SAT, 0.0, 20.0);
    fb0_bright_slider= fb0_opsfolder->addSlider(BRIGHT, 0.0, 20.0);
    
    fb0_hue_invert_toggle=fb0_opsfolder->addToggle(B_0+H_INVERT,0);
    fb0_saturation_invert_toggle=fb0_opsfolder->addToggle(B_0+S_INVERT,0);
    fb0_bright_invert_toggle=fb0_opsfolder->addToggle(B_0+B_INVERT,0);
    
    fb0_huex_mod_slider= fb0_opsfolder->addSlider(H_MOD, 0.0, 20.0);
    fb0_huex_offset_slider= fb0_opsfolder->addSlider(H_OFFSET, -20.0, 20.0);
    fb0_huex_lfo_slider= fb0_opsfolder->addSlider(H_LFO, -20.0, 20.0);
    
    fb0_x_displace_slider= fb0_opsfolder->addSlider(X_DISPLACE, -20.0, 20.0);
    fb0_y_displace_slider= fb0_opsfolder->addSlider(Y_DISPLACE, -20.0, 20.0);
    fb0_z_displace_slider= fb0_opsfolder->addSlider(Z_DISPLACE, 90, 110.0);
    fb0_rotate_slider=fb0_opsfolder->addSlider(ROTATE, -157.07, 157.07);
    
    
    
    fb0_hue_slider->bind(fb0_hue);
    fb0_saturation_slider->bind(fb0_saturation);
    fb0_bright_slider->bind(fb0_bright);
    fb0_huex_mod_slider->bind(fb0_huex_mod);
    fb0_huex_offset_slider->bind(fb0_huex_offset);
    fb0_huex_lfo_slider->bind(fb0_huex_lfo);
    fb0_x_displace_slider->bind(fb0_x_displace);
    fb0_y_displace_slider->bind(fb0_y_displace);
    fb0_z_displace_slider->bind(fb0_z_displace);
    fb0_rotate_slider->bind(fb0_rotate);
    //fb1
    ofxDatGuiFolder* fb1_opsfolder = guithings->addFolder("Buffer 1 ops", ofColor::white);
    fb1_hue_slider= fb1_opsfolder->addSlider(HUE, 0.0, 20.0);
    fb1_saturation_slider= fb1_opsfolder->addSlider(SAT, 0.0, 20.0);
    fb1_bright_slider= fb1_opsfolder->addSlider(BRIGHT, 0.0, 20.0);
    //
    fb1_hue_invert_toggle=fb1_opsfolder->addToggle(B_1+H_INVERT,0);
    fb1_saturation_invert_toggle=fb1_opsfolder->addToggle(B_1+S_INVERT,0);
    fb1_bright_invert_toggle=fb1_opsfolder->addToggle(B_1+B_INVERT,0);
    //
    fb1_huex_mod_slider= fb1_opsfolder->addSlider(H_MOD, 0.0, 20.0);
    fb1_huex_offset_slider= fb1_opsfolder->addSlider(H_OFFSET, -20.0, 20.0);
    fb1_huex_lfo_slider= fb1_opsfolder->addSlider(H_LFO, -20.0, 20.0);
    //
    fb1_x_displace_slider= fb1_opsfolder->addSlider(X_DISPLACE, -20.0, 20.0);
    fb1_y_displace_slider= fb1_opsfolder->addSlider(Y_DISPLACE, -20.0, 20.0);
    fb1_z_displace_slider= fb1_opsfolder->addSlider(Z_DISPLACE, 90, 110);
    fb1_rotate_slider=fb1_opsfolder->addSlider(ROTATE, -157.07, 157.07);
    //
    fb1_hue_slider->bind(fb1_hue);
    fb1_saturation_slider->bind(fb1_saturation);
    fb1_bright_slider->bind(fb1_bright);
    fb1_huex_mod_slider->bind(fb1_huex_mod);
    fb1_huex_offset_slider->bind(fb1_huex_offset);
    fb1_huex_lfo_slider->bind(fb1_huex_lfo);
    fb1_x_displace_slider->bind(fb1_x_displace);
    fb1_y_displace_slider->bind(fb1_y_displace);
    fb1_z_displace_slider->bind(fb1_z_displace);
    fb1_rotate_slider->bind(fb1_rotate);
    //cam1
    ofxDatGuiFolder* cam1_pixelfolder = guithings->addFolder("camera Pixelate", ofColor::white);
    cam1_pixel_toggle=cam1_pixelfolder->addToggle(CAMERA+TOGGLE,0);
    cam1_pixel_scale_slider=cam1_pixelfolder->addSlider(QUANTIZE,0,256);
    cam1_pixel_mix_slider=cam1_pixelfolder->addSlider(MIX,-2,2);
    cam1_pixel_brightscale_slider=cam1_pixelfolder->addSlider(B_SCALE,-10,10);
    //
    cam1_pixel_scale_slider->bind(cam1_pixel_scale);
    cam1_pixel_mix_slider->bind(cam1_pixel_mix);
    cam1_pixel_brightscale_slider->bind(cam1_pixel_brightscale);
    //fb0
    ofxDatGuiFolder* fb0_pixelfolder = guithings->addFolder("Buffer 0 Pixelate", ofColor::white);
    fb0_pixel_toggle=fb0_pixelfolder->addToggle(B_0+TOGGLE,0);
    fb0_pixel_scale_slider=fb0_pixelfolder->addSlider(QUANTIZE,0,256);
    fb0_pixel_mix_slider=fb0_pixelfolder->addSlider(MIX,-2,2);
    fb0_pixel_brightscale_slider=fb0_pixelfolder->addSlider(B_SCALE,-10,10);
    //
    fb0_pixel_scale_slider->bind(fb0_pixel_scale);
    fb0_pixel_mix_slider->bind(fb0_pixel_mix);
    fb0_pixel_brightscale_slider->bind(fb0_pixel_brightscale);
    //fb1
    ofxDatGuiFolder* fb1_pixelfolder = guithings->addFolder("Buffer 1 Pixelate", ofColor::white);
    fb1_pixel_toggle=fb1_pixelfolder->addToggle(B_1+TOGGLE,0);
    fb1_pixel_scale_slider=fb1_pixelfolder->addSlider(QUANTIZE,0,256);
    fb1_pixel_mix_slider=fb1_pixelfolder->addSlider(MIX,-2,2);
    fb1_pixel_brightscale_slider=fb1_pixelfolder->addSlider(B_SCALE,-10,10);
    //
    fb1_pixel_scale_slider->bind(fb1_pixel_scale);
    fb1_pixel_mix_slider->bind(fb1_pixel_mix);
    fb1_pixel_brightscale_slider->bind(fb1_pixel_brightscale);
    //registertolistentoevents
    guithings->onButtonEvent(this, &GuiApp::onButtonEvent);
    padArea = new ofRectangle(0, 0, pad_W, pad_W);
    // X/Y Pads
    guithings->add2dPad(B_0+XY, *padArea);
    guithings->addBreak();
    guithings->add2dPad(B_1+XY, *padArea);
    // Listen to pad events
    guithings->on2dPadEvent(this, &GuiApp::on2dPadEvent);
    guithings->addBreak();
    //
    ofxDatGuiLog::quiet();
    
  }

    // Pad event handler
    void GuiApp::on2dPadEvent(ofxDatGui2dPadEvent e)
    {
    //   negative translation: -(x_displace_bound*(half_width-e.x)/100)
    //
    //   -: minus moves left/down
    //
    //   x_displace_bound: user defined upper bound
    //
    //   *: multiply by <1 to reduce value
    //
    //   half_width-e.x: capture the difference between input and middle
    //
    //   /100: express as percentage
    //
    //   positive translation: x_displace_bound*((e.x/half_width)-1)
    //
    //   positive x_displace moves right/up
    //
    //   x_displace_bound: user defined upper bound
    //
    //   *: multiply by >1 to increase value
    //
    //   e.x/half_width: e.x will always be greater than half_width, so result will be 1.(e.x)
    //
    //   -1: grab percantage increase by reducing result by 1
     
        // Buffer 0 X/Y
        if (e.target->getLabel() ==B_0+XY) {
            if (e.x < half_width) {
                // negative x
                fb0_x_displace_slider->setValue(-(x_displace_bound*(half_width-e.x)/100));
            }
            if (e.x > half_width) {
                // positive x
                fb0_x_displace_slider->setValue(x_displace_bound*((e.x/half_width)-1));
            }
            if (e.y < half_width) {
                // negative x
                fb0_y_displace_slider->setValue(-(y_displace_bound*(half_width-e.y)/100));
            }
            if (e.y > half_width) {
              // positive x
                fb0_y_displace_slider->setValue(y_displace_bound*((e.y/half_width)-1));
            }
        }
        // Buffer 1 X/Y
        if (e.target->getLabel() ==B_1+XY) {
          if (e.x < half_width) {
              // negative x
              fb1_x_displace_slider->setValue(-(x_displace_bound*(half_width-e.x)/100));
          }
          if (e.x > half_width) {
              // positive x
              fb1_x_displace_slider->setValue(x_displace_bound*((e.x/half_width)-1));
          }
          if (e.y < half_width) {
              // negative x
              fb1_y_displace_slider->setValue(-(y_displace_bound*(half_width-e.y)/100));
          }
          if (e.y > half_width) {
            // positive x
              fb1_y_displace_slider->setValue(y_displace_bound*((e.y/half_width)-1));
          }
              }
    }

    void GuiApp::onDropdownEvent(ofxDatGuiDropdownEvent e)
    {
        cout << "onDropdownEvent: " << e.target->getLabel() << " Selected" << endl;
        //
        // Select channel
        //
        if(e.target->getLabel()==CH1+NONE){
            channel1=0;
        }
        if(e.target->getLabel()==CH1+CAMERA){
            channel1=1;
        }
        if(e.target->getLabel()==CH1+SYPHONER){
            channel1=2;
        }
        //
        // Buffer 0 Mix options
        //
        if(e.target->getLabel()==B_0+NONE){
            FBmix=0;
        }
        if(e.target->getLabel()==B_0+BLEND){
            FBmix=1;
        }
        if(e.target->getLabel()==B_0+KEY){
            FBmix=2;
        }
        //
        // Buffer 1 Mix options
        //
        if(e.target->getLabel()==B_1+NONE){
            FB1mix=0;
        }
        if(e.target->getLabel()==B_1+BLEND){
            FB1mix=1;
        }
        if(e.target->getLabel()==B_1+KEY){
            FB1mix=2;
        }
    }


//--------------------



void GuiApp::onButtonEvent(ofxDatGuiButtonEvent e)
{
    // Toggle tetra
    if(e.target->getLabel()==TETRA){tetrahedron_switch=!tetrahedron_switch;}
    // Toggle cam pixelation
    if(e.target->getLabel()==CAMERA+TOGGLE){cam1_pixel_switch=!cam1_pixel_switch;}
    // toggle buffer 0 pixelation
    if(e.target->getLabel()==B_0+TOGGLE){fb0_pixel_switch=!fb0_pixel_switch;}
    // toggle buffer 1 pixelation
    if(e.target->getLabel()==B_1+TOGGLE){fb1_pixel_switch=!fb1_pixel_switch;}
    // camera horizontal mirror
    if(e.target->getLabel()==CAMERA+H_MIRROR){cam1_hflip_switch=!cam1_hflip_switch;}
    // camera vertial mirror
    if(e.target->getLabel()==CAMERA+V_MIRROR){cam1_vflip_switch=!cam1_vflip_switch;}
    // buffer 0 horizontal mirror
    if(e.target->getLabel()==B_0+H_MIRROR){fb0_hflip_switch=!fb0_hflip_switch;}
    // buffer 0 vertial mirror
    if(e.target->getLabel()==B_0+V_MIRROR){fb0_vflip_switch=!fb0_vflip_switch;}
    // buffer 1 horizontal mirror
    if(e.target->getLabel()==B_1+H_MIRROR){fb1_hflip_switch=!fb1_hflip_switch;}
    // buffer 0 vertial mirror
    if(e.target->getLabel()==B_1+V_MIRROR){fb1_vflip_switch=!fb1_vflip_switch;}
    // buffer 0 hue invert
    if(e.target->getLabel()==B_0+H_INVERT){fb0_hue_invert=!fb0_hue_invert;}
    // buffer 0 sat invert
    if(e.target->getLabel()==B_0+S_INVERT){fb0_saturation_invert=!fb0_saturation_invert;}
    // buffer 0 bright invert
    if(e.target->getLabel()==B_0+B_INVERT){fb0_bright_invert=!fb0_bright_invert;}
    // buffer 1 hue invert
    if(e.target->getLabel()==B_1+H_INVERT){fb1_hue_invert=!fb1_hue_invert;}
    // buffer 1 sat invert
    if(e.target->getLabel()==B_1+S_INVERT){fb1_saturation_invert=!fb1_saturation_invert;}
    // buffer 1 bright invert
    if(e.target->getLabel()==B_0+B_INVERT){fb1_bright_invert=!fb1_bright_invert;}
    // output to Syphoner
    if(e.target->getLabel() == SYPHON_OUT){ syphonOutput=!syphonOutput;}

    // toggle ch1 hue power map
    if(e.target->getLabel() == CH1+H_POW_MAP){
        ch1hue_powmaptoggle =! ch1hue_powmaptoggle;
    }
    // toggle ch1 sat power map
    if(e.target->getLabel() == CH1+S_POW_MAP){
        ch1sat_powmaptoggle =! ch1sat_powmaptoggle;
    }
    // toggle ch1 bright power map
    if(e.target->getLabel() == CH1+B_POW_MAP){
        ch1bright_powmaptoggle =! ch1bright_powmaptoggle;
    }
    // toggle ch1 sat wrap
    if(e.target->getLabel() == CH1+WRAP_SAT){
        channel1satwrap =! channel1satwrap;
    }
    // toggle ch1 bright wrap
    if(e.target->getLabel() == CH1+WRAP_BRIGHT){
        channel1brightwrap =! channel1brightwrap;
    }
}

void GuiApp::newMidiMessage(ofxMidiMessage& message) {

    // because we are binding on the event, we don't need a queue.
    
    // we can use ->setValue() & ->update(); OR [value]=newValue & ->bind()
    
   /* AKAI MPKmini2 mappings
    
    Read from the ofxMidi midiInExample project.
    
     Pads A 1 - 8: channel 10, ctrl 1-8
    
    Pads B 1 - 8: channel 10, ctrl 9-16
    
    Knobs 1 - 8:  channel 1, ctrl 16-23
    
    joystick L/R: Pitch Bend, channel 1, val:0-16k
    
    joystick U/D: channel 1, ctrl 1
    
    keys: pitch 48-72
    
    */

   if(message.status < MIDI_SYSEX) {
    // float message.value so that we can operate on it later.
    float m_val = message.value;
       
       if(message.status == MIDI_CONTROL_CHANGE) {
           // Knobs 1 - 8
           // value: 0-127
           //(message.value-63.0)/63.0
            if (message.control == 66) {
            // fb 0 brightness
            fb0_bright=10+(message.value-63.0)/63.0;
            }
            if (message.control == 2) {
            // fb 0 hue mod
            fb0_huex_mod=10+(message.value-63.0)/63.0;
            }
            if (message.control == 3) {
            // fb 0 hue offset
            fb0_huex_offset=((message.value-63.0)/63.0);
            }
            if (message.control == 4) {
            // fb 0 hue lfo
            fb0_huex_lfo=(message.value-63.0)/63.0;
            }
            if (message.control == 5) {
            // fb 1 brightness
            fb1_bright=10+(message.value-63.0)/63.0;
            }
            if (message.control == 6) {
            // hue mod
            fb1_huex_mod=10+(message.value-63.0)/63.0;
            }
            if (message.control == 7) {
            // hue offset
            fb1_huex_offset=((message.value-63.0)/63.0);
            }
            if (message.control == 8) {
            // hue lfo
            fb1_huex_lfo=(message.value-63.0)/63.0;
            }
           
           
           // Pads: ctrl 20-35
           
           // Pad A: 20 - 27;
           if (message.control == 20) {}
           if (message.control == 21) {}
           if (message.control == 22) {}
           if (message.control == 23) {}
           if (message.control == 24) {}
           if (message.control == 25) {}
           if (message.control == 26) {}
           if (message.control == 27) {}
           // Pad B: 28 - 35;
           if (message.control == 28) {}
           if (message.control == 29) {}
           if (message.control == 30) {}
           if (message.control == 31) {}
           if (message.control == 32) {}
           if (message.control == 33) {}
           if (message.control == 34) {}
           if (message.control == 35) {}
           
           
           // Joystick: CC2 on both X and Y axis.
           if (message.control == 69) {
               // y axis joystick, up
               fb0_y_displace=m_val/127;
           }
           if (message.control == 96) {
               // y axis joystick, down
               fb0_y_displace=-(m_val/127);
           }
           if (message.control == 24) {
               // x axis joystick, right
               fb0_x_displace=-(m_val/127);
           }
           if (message.control == 42) {
               // y axis joystick, left
              fb0_x_displace=m_val/127;
           }
           // End of message.status == MIDI_CONTROL_CHANGE block
       } else if (message.status == MIDI_NOTE_ON) {
           // Keyboard is bound to fb0 luma key value.
           float m_pitch = message.pitch;
           fb0lumakeyvalue=m_pitch/120;
       }


}

}


//----------------------------------
    void GuiApp::update() {
       
    }
void GuiApp::exit() {
    
    // clean up
//    midiMessages.erase(midiMessages.begin());
    midiIn.closePort();
    midiIn.removeListener(this);
}


//------------------------------
    void GuiApp::draw() {
    
    }
//---------------------------




