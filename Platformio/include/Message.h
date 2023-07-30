#pragma once

#include <Arduino.h>
#include "Image.h"

// Since this project is unique to my board, I'll just 
// hard code the LED pin designations since it allows
// me to take advantage of port manipulation for faster
// switching times

#define MAX_STORED_IMAGES 36

class Message {
public:

    // Set the LEDs to their initial state
    void init(){
        setupLEDs();        
        allLEDsOff();
    }

    // Set the LED pins to output configuratino
    void setupLEDs(){
        DDRF |= B11110010;
        DDRD |= B01111100;
    }

    // Turn all row pins low and all column pins high
    void allLEDsOff(){
        PORTF &= B00001101;
        PORTD |= B01111100; 
    }

    // Set on the stored images at a given index
    bool setImage(unsigned idx, uint32_t image){
        if (idx >= MAX_STORED_IMAGES) return false;
        num_stored_images_ = max(idx + 1, num_stored_images_);
        images_[idx] = image;
        return true;
    }

    // Set one of the stored images at the next index
    bool addImage(uint32_t image){
        if (num_stored_images_ >= MAX_STORED_IMAGES) return false; 
        images_[num_stored_images_++] = image;
        return true;
    }

    // Image getter
    Image getImage(unsigned idx){
        return images_[idx];
    }

    // Given an image, show it on the LED array for a set duration
    void illuminateImage(const Image& image, unsigned long duration_millis){
        unsigned long start = millis();

        while (millis() - start < duration_millis){
            for (int frame_idx = 0; frame_idx < image.num_frames; frame_idx++){
                const Frame& frame = image.frames[frame_idx];
                PORTF = (PORTF & 0b00001101) | rowMask(frame);
                PORTD = (PORTD & 0b10000011) | colMask(frame);
                delay(1);
                allLEDsOff();
            }
        }
    }

    // Display each stored image in order for a certain duration each. Optionally scroll between images
    // and have a extra pause equal to the image delay on the first image 
    void playMessage(unsigned long duration_millis, bool scroll = false, bool initial_pause = false){
        if (num_stored_images_ == 0) return;

        if (initial_pause){
            illuminateImage(images_[0], duration_millis);
        }
        
        for (unsigned idx = 0; idx < num_stored_images_; idx++){
            Image rendered_image(images_[idx]);

            if (scroll){
                illuminateImage(rendered_image, duration_millis/5);
                for (int i = 0; i < 5; i++){
                    uint32_t next_image_pixels = idx == (num_stored_images_ - 1) ? 0 : images_[idx+1];
                    uint32_t pixels_to_render = shiftInFromRight(leftShift(images_[idx], i+1), next_image_pixels, i);
                    rendered_image.setActivePixels(pixels_to_render);
                    illuminateImage(rendered_image, duration_millis/5);
                }
            }
            
            else{
                illuminateImage(rendered_image, duration_millis);
            }
            
        }
    }

private:

    unsigned num_stored_images_ = 0;
    uint32_t images_[MAX_STORED_IMAGES];

    // Given the 5-bit mask of LEDs, return an 8-bit mask for the row port
    uint8_t rowMask(Frame frame){
        static const uint8_t row1 = B00000010;
        static const uint8_t row2 = B00010000;
        static const uint8_t row3 = B00100000;
        static const uint8_t row4 = B01000000;
        static const uint8_t row5 = B10000000;

        uint8_t mask =
              (row1 * ((frame.row >> 4) & 0x01)) 
            | (row2 * ((frame.row >> 3) & 0x01))
            | (row3 * ((frame.row >> 2) & 0x01))
            | (row4 * ((frame.row >> 1) & 0x01))
            | (row5 * ((frame.row >> 0) & 0x01));

        return mask;
    }

    // Given the 5-bit mask of LEDs, return an 8-bit mask for the column port
    uint8_t colMask(Frame frame){
        static const uint8_t row1 = B00000100;
        static const uint8_t row2 = B00001000;
        static const uint8_t row3 = B00010000;
        static const uint8_t row4 = B00100000;
        static const uint8_t row5 = B01000000;

        uint8_t mask = 
            (row1 * ((frame.col >> 4) & 0x01)) 
          | (row2 * ((frame.col >> 3) & 0x01))
          | (row3 * ((frame.col >> 2) & 0x01))
          | (row4 * ((frame.col >> 1) & 0x01))
          | (row5 * ((frame.col >> 0) & 0x01));

        // Column mask is inverted because the columns sink current instead of sourcing it
        return ~mask;
    }

    // Shift a pixel representation to the left, discarding pixels that go off the screen
    uint32_t leftShift(uint32_t image, int num_steps){
        static constexpr uint32_t clearing_mask = 0b10000'10000'10000'10000'10000;

        uint32_t final_mask = 0b11111'11111'11111'11111'11111;
        for (int step = 0; step < num_steps; step++){
            final_mask &= ~(clearing_mask >> step);
        }

        return (image & final_mask) << num_steps;
    }

    // Given an initial image, shift another image in from the right and overlay it over the original
    uint32_t shiftInFromRight(uint32_t im1, uint32_t im2, int num_steps){
        static constexpr uint32_t keeping_mask = 0b10000'10000'10000'10000'10000;

        uint32_t final_mask = 0x0000;
        for (int step = 0; step < num_steps; step++){
            final_mask |= (keeping_mask >> step);
        }

        return im1 | ((im2 & final_mask) >> (5 - num_steps));
    }
};

