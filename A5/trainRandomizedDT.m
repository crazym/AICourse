%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% CSC D84 - Assignment 6 - Decision tree training
%
% This is a recursive function. It performs the training for the randomized
% decision trees. Because of the randomized nature of the training, the
% process is pretty straightforward.
%
% Trees here will be stored in arrays (just like the heaps in B63!). 
%
% Trees are thus stored in an array of size (2^(levels+1))-1 x 13. 
% 
% They are stored in the same order as nodes in a heap, that is:
%
%			  			 1
%		      		  2      3
%                   4  5    6  7
% and so on.
%
% Each node performs tests on a set of input samples. 
%
% At each node we store: [index_1, index_2, test_id, classDistribution]
%
% index_1 and index_2 are the indices of the pixels in the sample that
% the test compares.
%
% The test_id is a numeric identifier that indicates what test is
% being performed (see test_pixels.m). 
%
% Class distribution is the distribution of data at that particular node
% given the dataset it is working with, i.e. it's a histogram with 10
% bins, where each bin tells you how many samples of the corresponding
% class are present in the set of samples the node is working with.
%
% For the leaves, it is proportional to the the probability that a sample
% from class 'i' will end up at that leaf given the random tests 
% performed by the tree. 
%
% Trees are built recursively, each test splits the dataset into two
% smaller subsets, and calls the tree construction function on these
% smaller subsets to get the tests further down the tree. 
%
% Starter code: F.J.E., March 18, 2014
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [Tree]=trainRandomizedDT(Tree,trainS,trainC,idx, trials, levels)
% Tree (intially empty) -------------|    |      |    |      |        |
% Training samples -----------------------|      |    |      |        |
% Training classes ------------------------------|    |      |        |
% Index of this node within the tree -----------------|      |        |
% Number of random tests to consider/node -------------------|        |
% Number of levels in the tree (starting at zero) --------------------|

if (idx==1)
  % Initialize an empty tree
  % Initially, the pixel indexes and test ids are -1
  % And all class probability distributions are uniform.
  Tree=zeros((2^(levels+1))-1,13);
  Tree(:,1)=-1;
  Tree(:,2)=-1;
  Tree(:,3)=-1;
  ClassDistrib=ones(1,10)/10;
  for i=1:size(Tree,1)
	Tree(i,4:end)=ClassDistrib;
  end;
end;

% Compute class distribution for this level - if index is in the second half 
% of the tree array, then this is a leaf node and we return immediately 
% (no test to be performed here!)

classDistrib=hist(trainC,[1:10]);
Tree(idx,4:end)=classDistrib;
if (idx>size(Tree,1)/2 | length(trainC)==0) return; end;

fprintf(2,'Training randomized tree at index %d\n',idx);

% This is not a leaf node. Try a number of randomly selected tests, 
% and choose the one that gives that maximizes the dissimilarity
% between label distributions for the left and right subsets of training
% cases after splitting.

MaxTestID = 0;		% <---- You HAVE to change this to reflect the number of tests your
					%	    code can choose from. See test_pixels.m

%% Your code will be updating the variables just below
maxDis = 0;			% Maximum dissimilarity for splits found so far	
bestLeftSplit=[];	% Left-side data for best split 
bestLeftClass=[];	% Left-side classes for best split
bestRightSplit=[];	% Right-side data for best split
bestRightClass=[];	% Right-side classes for best split
best_pix1=-1;		% Indices of pixels and tests used to create the best split
best_pix2=-1;		
best_testid=-1;		% Id of the best test found

for tr=1:trials		% For the specified number of random tests to try

	LeftSplit=[];
	RightSplit=[];
	LeftClassPDF=zeros(1,10);
	RightClassPDF=zeros(1,10);


	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	%  TO DO:
	%		 Write the code to train a randomized decision tree. The code must
	%		 try 'trials' different randomly selected tests (on randomly selected
	%		 entries on each training sample). For each test, the right and left
	%		 split are computed (i.e. the subsets of samples being sent to the
	%		 right vs. left children of the current node, for which we are 
	%		 choosing the test).
	%
	%		 Given the splits (both the samples and the class labels must be
	%		 split into subsets). Determine how good the test is.
	%
	%		 For this, remember our definition of 'reduction in uncertainty',
	%		 and consider that a good test should give you class distributions
	%		 for the left and right node that are as different as possible, while
	%		 also making sure that the data is as evenly split as possible.
	%
	%		 You should do a bit of research into ways for estimating the 
	%		 similarity between two probability distributions.
	%	
	%		 However you do it, your code must choose the 'best' test among all
	%		 the ones you tried. Set the best splits, best class label vectors,
	%		 best test index, and so on according to this test. Fill-up the
	%		 three first entries for this node in the tree (the pixel id 1,
	%		 pixel id 2, and test id). And then call recursively with the
	%		 corresponding subset of samples, labels, and the correct index
	%		 for the left and right children of this node.
	%
	%		 Important Note: When creating the splits. Samples that 'pass' the
	%		 test (those returned in a vector of indexes by test_pixels.m)
	%		 will go to the *left* child node, while the complement (those that do
	%		 not pass the test) will go in the *right* child node.
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	N = size(trainS, 1); % sample set size
	chosenTest = randperm(MaxTestID)(1);
	randPixes = randperm(size(trainS, 2));
	% pass in random pix1 and pix2 over the whole training sample
	left_indices = test_pixels(trainS(:, randPixes(1)), trainS(:, randPixes(2)), chosenTest);
	% right indices are the complement of left_indices
	right_indices = setdiff(1:N, left_indices);

	LeftNum = size(left_indices, 1);
	RightNum = size(right_indices, 1);
	LeftSplit = trainS(left_indices);
	RightSplit = trainS(right_indices);

	LeftClassPDF = hist(trainC(left_indices), [1:10]);
	RightClassPDF = hist(trainC(right_indices), [1:10]);

	entropy_before = 0;
	entropy_left = 0;
	entropy_right = 0;
	% notive that addition are used in the loop, all values negated after the loop
	for i=1:10
		classNum = size(find(trainC==i), 1);
		entropy_before = entropy_before + (classNum/N) * log2(classNum/N);

		entropy_left = entropy_left + (LeftClassPDF(i)/LeftNum) * log(LeftClassPDF(i)/LeftNum);
		entropy_right = entropy_right + (RightClassPDF(i)/RightNum) * log(RightClassPDF(i)/RightNum);
	end;

	entropy_after = (LeftNum/N) * (-entropy_left) + (RightNum/N) * (-entropy_right);

	InforGain = - entropy_before - entropy_after;

	if (InforGain>maxDis)
		maxDis = InforGain;
		bestLeftSplit = LeftSplit;
		bestRightSplit = RightSplit;
		bestLeftClass = LeftClassPDF;
		bestRightClass = RightClassPDF;
		best_pix1 = randPixes(1);
		best_pix2 = randPixes(2);
		best_testid = chosenTest;
	end;
end;

% Completed selection of random test. Store the test for this node, and call recursively with the best
% splits generated by the testing above

Tree(idx,1)=best_pix1;
Tree(idx,2)=best_pix2;
Tree(idx,3)=best_testid;

[Tree]=trainRandomizedDT(Tree,bestLeftSplit,bestLeftClass,2*idx,trials, levels-1);
[Tree]=trainRandomizedDT(Tree,bestRightSplit,bestRightClass,(2*idx)+1,trials, levels-1);
return;

