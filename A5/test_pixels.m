%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% CSC D84 - Assignment 6 - test definitions for randomized tree forests.
%
% This function contains all possible tests that can be performed 
% between pairs of pixels. You will have to define your own, and you can use
% as many as you can think of. It is likely that having more varied tests will
% give you stronger classification results.
%
% Keep in mind that pixel brightness data is in [0-255] and you will run tests
% on either pairs of pixels or individual pixels.
%
% Make sure you update your MaxTestID value in trainRandomizedDT.m to reflect
% how many tests are available in here.
%
% * ALL TESTS ARE BINARY * because we have to split the training data. So,
% this function simply returns the indices of those entries in the pixel
% value lists that 'pass' the test (the complement goes to the opposite
% child node)
%
% Please note that pix1 and pix2 are vectors with ALL the pairs of pixels for
% all training patches. Try to do your comparisons and tests using vectors
% instead of for loops - much, MUCH faster.
%
% Starter code: F.J.E., March 18, 2014
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [idx1]=test_pixels(pix1,pix2,testID)

	switch(testID)
		case 1
			% black - white
			idx1=find(pix1 > 225 & pix2 == 0);			
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		%	TO DO:
		%		  Add your own tests. You can add as many as you like, but
		%		 think about why a specific test would be helpful (i.e. don't
		%		 just add random junk here). 
		%
		%		  Unless you want to wait forever, make sure your tests use
		%		 vector operations and matlab functions such as 'find'. Type
		%		 'help find' to learn more.
		%
		%         Said a different way. There should be *no* for loops in this
		%		 script.
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		case 2 
			% both  black
			idx1 = find(pix1 > 225 & pix2 > 225);
		case 3
			% both white
			idx1 = find(pix1 == 0 & pix1 == 0);
		case 4
			% white - black
			idx1 = find(pix1 == 0 & pix2 > 225);
		case 5
			% both moderate (80 - 120)
			idx1 = find(abs(pix1-100)<20 & abs(pix2-100)<20);
		case 6
			% both in (180 - 220)
			idx1 = find(abs(pix1-200)<20 & abs(pix2-200)<20);
		case 7
			% both around 150 (120 - 180)
			idx1 = find(abs(pix1-150)<30 & abs(pix2-150)<30);
		otherwise
			fprintf(2,'Invalid testID!\n'); idx1=[]; return;
	end;

        return;

