#pragma once

#include <Arduino.h>

/* 
 * OVERVIEW
 *
 * The ATMega32u4 can only support 40mA out of any individual IO port. Therefore
 * multilplexing is required to keep the instantaneous current in each port low
 * since each LED draws 20mA when fully illuminated. This means that only two 
 * rows and columns of the LED array can be active at once
 */


/* Basic struct indicating which LED to light in each row and column
   Bits are organized left to right to mimic the physical LEDs, so
   a row value of 0b10100 indicates that LEDs 0 and 2 are to be lit */
struct Frame{
    uint8_t row : 5;
    uint8_t col : 5;
};


/* The Image class takes in a certain arrangement of pixels as input in the form
   of a 32-bit unsigned integer ,of which the lowest 25 bits correspond to the 
   25 LEDs on the board. It then calculates a series of frames which can be 
   multiplexed together such that no more than 2 LEDs in each row/column are ever
   lit at once */
class Image{
public:
    // Default constructor to allow arrays of images
    Image() = default;

    // Value based constructor
    Image(uint32_t bits){
        calculate(bits);
    }

    // Set value post-construction. Useful for arrays of images
    void setActivePixels(uint32_t bits){
        calculate(bits);
    }

    // There will never be more than 13 frames to write an image
    Frame frames[13];
    
    // How many frames in this image
    int num_frames = 0;

private:
    // An LED is lit when a row-column pair are both active. This struct represents such a pair
    struct Pair{
        bool found = false;
        int row; 
        int col;
    };

    // Given one row and two columns, or two rows and one column, see if we can add another row/column
    // to make it a square of LEDs
    void makeSquare(uint32_t& active_pixels, Pair& pair_1, Pair& pair_2, bool row_duplicate){
        int start_val = row_duplicate ? pair_2.row : pair_2.col;
        
        for (int new_idx = start_val; new_idx < 5; new_idx++){
            int idx1 = row_duplicate ? (new_idx * 5 + pair_1.col) : (pair_1.row * 5 + new_idx);
            int idx2 = row_duplicate ? (new_idx * 5 + pair_2.col) : (pair_2.row * 5 + new_idx);

            uint32_t mask_1 = (1UL << 24) >> idx1;
            uint32_t mask_2 = (1UL << 24) >> idx2;
            if ((active_pixels & mask_1) && (active_pixels & mask_2)){
                if (row_duplicate) 
                    pair_2.row = new_idx;
                else
                    pair_2.col = new_idx;
                active_pixels &= ~mask_1;
                active_pixels &= ~mask_2;
                return;
            }
        }
    }

    // Given a the indices of active LEDs, return a 5-bit mask the rows and columns
    Frame makeFrame(const Pair& p1, const Pair& p2){
        Frame output;
        
        if (p1.found){
            output.row = (0b10000 >> p1.row); 
            output.col = (0b10000 >> p1.col);
        }

        if (p2.found){
            output.row |= (0b10000 >> p2.row);
            output.col |= (0b10000 >> p2.col);
        }

        return output;
    }

    // Calculate the frames necessary to draw an image
    void calculate(uint32_t active_pixels){

        // Helper function for checking if a given pixel is lit
        auto isPixelLit = [&active_pixels](uint8_t pixel_idx) -> bool {
            uint32_t mask = (1UL << (24 - pixel_idx));
            bool pixel_is_lit = mask & active_pixels;
            active_pixels &= ~mask; // Remove pixel from list of active pixels if its lit
            return pixel_is_lit;
        };

        // Get the set of 1, 2, or 4 LEDs for each of the (up to) 13 frames 
        uint8_t starting_pixel_idx = 0;
        for (int frame = 0; frame < 13; frame++){
            Pair pair_1;
            Pair pair_2;
            bool row_duplicate = false;

            // Find an active pixel
            for (int pixel_idx = starting_pixel_idx; pixel_idx < 25; pixel_idx++, starting_pixel_idx++){

                // Determine if this LED is on or off
                if (isPixelLit(pixel_idx)){

                    // Determine the row and column that this pixel is in
                    int row = pixel_idx / 5;
                    int col = pixel_idx % 5;

                    // If this is the first one we find
                    pair_1.found = true;
                    pair_1.row = row;
                    pair_1.col = col;

                    // Delete this entry in active pixels
                    break;
                }
            }

            // If didn't find a pixel, then we're done and the image is complete
            if (not pair_1.found){
                num_frames = frame;
                break;
            } 


            // Check for another pixel in the same row
            for (uint8_t col_idx = pair_1.col + 1; col_idx < 5; col_idx++){
                uint8_t pixel_idx = pair_1.row * 5 + col_idx;

                if (isPixelLit(pixel_idx)){
                    row_duplicate = true;
                    pair_2.found = true;
                    pair_2.row = pair_1.row;
                    pair_2.col = col_idx;

                    break;
                }
            }

            // If we did not find another in the row, also check the column
            if (not pair_2.found){
                for (uint8_t row_idx = pair_1.row + 1; row_idx < 5; row_idx++){
                    uint8_t pixel_idx = row_idx * 5 + pair_1.col;

                    if (isPixelLit(pixel_idx)){
                        row_duplicate = false;
                        pair_2.found = true;
                        pair_2.row = row_idx;
                        pair_2.col = pair_1.col;

                        break;
                    }
                }
            }

            // If we found two active LEDs, see if we can make it a square on any other row/column
            if (pair_2.found){
                makeSquare(active_pixels, pair_1, pair_2, row_duplicate);
            }

            // Record the calculated frame
            frames[frame] = makeFrame(pair_1, pair_2);

        } // end for frame
    }
};