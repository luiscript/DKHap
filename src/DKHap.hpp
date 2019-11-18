/*
 Copyright (C) 2018 Luis Fernando García [http://luiscript.com]
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#ifndef DKHap_hpp
#define DKHap_hpp

#include "ofMain.h"
#include "DKModule.hpp"

#include "ofxHapPlayer.h"

class DKHap: public DKModule
{
private:
    ofxHapPlayer player;
    bool videoReady;
    float videoPosition;
    bool isPlaying;
public:
    void setup()
    {
        config();
    };
    void setup(string videoFile)
    {
        player.load(videoFile);
        config();
    };
    void config()
    {
        videoPosition = 0;
        isPlaying = videoReady = false;
    };
    void update()
    {
        isPlaying ? player.update() : player.setPosition(videoPosition);
        videoPosition = isPlaying ? player.getPosition() : videoPosition;
    };
    void draw()
    {
        if(videoReady)
        {
            ofBackground(0, 0, 0);
            ofDisableLighting();
            player.draw(0,0);
        }
    };
    void addModuleParameters()
    {
        gui->addButton("file")->onButtonEvent(this, &DKHap::onVideoOpen);
        gui->addButton("play")->onButtonEvent(this, &DKHap::onVideoPlay);
        
        ofxDatGuiSlider * posSlider = gui->addSlider("position", 0, 1);
        posSlider->setPrecision(4)->bind(videoPosition);
        posSlider->onSliderEvent(this, &DKHap::onVideoPositionChange);
    };
    void onVideoOpen(ofxDatGuiButtonEvent e)
    {
        ofFileDialogResult openFileResult = ofSystemLoadDialog("Select hap video");
        
        //Check if the user opened a file
        if (openFileResult.bSuccess){
            processOpenFileSelection(openFileResult);
        }
    }
    
    void onVideoPlay(ofxDatGuiButtonEvent e)
    {
        if(isPlaying)
        {
            player.stop();
            gui->getButton("play")->setLabel("play");
        } else
        {
            player.play();
            gui->getButton("play")->setLabel("stop");
        }
        isPlaying = !isPlaying;
    }
    
    void processOpenFileSelection(ofFileDialogResult openFileResult)
    {
        ofFile file (openFileResult.getPath());
        
        if (file.exists()){
            
            string fileExtension = ofToUpper(file.getExtension());
            
            if (fileExtension == "MOV")
            {
                player.load(openFileResult.getPath());
                videoReady = true;
                player.play();
                gui->getButton("play")->setLabel("stop");
                isPlaying = true;
            }
        }
        
    }
    
    void onVideoPositionChange(ofxDatGuiSliderEvent e)
    {
        player.setPosition(e.scale);
    }
    
    void loadFile(string file)
    {
        player.load(file);
        videoReady = true;
        player.play();
        gui->getButton("play")->setLabel("stop");
        isPlaying = true;
    }
};


#endif /* DKHap_hpp */
