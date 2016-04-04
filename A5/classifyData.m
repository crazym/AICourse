%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% CSC D84 - Assignment 6 - Classification using a random tree forest
%
% This function classifies a set of samples using a trained random
% tree forest. It outputs a confusion matrix, which is a 10x10
% matrix in which entry M(i,j) gives you the number of times that
% a sample whose correct class was 'i' was classified by the 
% random tree forest as being of class 'j'. 
%
% Which entries in this matrix should be large if the classifier is
% working?
%
% Note that you can run this both on the training and the testing
% datasets, but we only really care about results on test data.
%
% Starter code: F.J.E., March 18, 2014
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [sampleCM]=classifyData(samples,classes,forest)
% input samples ----------------------|       |       |
% patch classes so we can compute accuracy ---|       |
% Trained tree forest --------------------------------|

N=size(forest,1);    % Number of trees in this forest
sampleCM=zeros(10,10);    % Sample confusion matrix

fprintf(2,'Forest size=%d, Processing sample: ',N);
for i=1:size(samples,1);        % For all patches
    pch=samples(i,:);                % This is sample i. 
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   	% TO DO:
    %         Complete the classification function. Your code must
    %         test the current sample by passing it through each
    %         of the decision trees in the tree forest. Once a
    %         terminal node is reached (leaf level node, or a node
    %         whose test is '-1') the leaf node 'votes' using its
    %         class distribution. e.g. if the current leaf node's
    %         class distribution is [1 0 2 5 0 2 3 1 9] then the
    %         node adds 1 vote to class 0, 0 to class 1, 2 to class 2,
    %         and so on.
    %
    %         Once you have tested using all trees in the forest,
    %         determine the most likely class for this sample, and
    %         use the true class and your estimated class to update
    %         the confusion matrix sampleCM
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    classes_pch = zeros(1, 10);
    % get the class voting
    for treeId=1:N
    	tree = forest(treeId);
    	nodeId = 1;
    	while (nodeId<(size(tree,1)/2) | tree(nodeId)(3) ~= -1)
    		node = tree(nodeId);
    		result = test_pixels(pch(node(1)), pch(node(2)), node(3));
    		if result == 1
    			nodeId == 2*nodeId;
    		else
    			nodeId == 2*nodeId+1;
    		end
    	end
    	classes_pch = classes_pch + tree(nodeId)(4:end);
  	end
  	% find the best guess
  	cls = find(classes_pch==max(classes_pch));
  	if cls == classes(i)
  		% correct classification, add 1 to the diagonal
  		sampleCM(cls,cls) = sampleCM(cls,cls) + 1;
  	else
  		sampleCM(classes(i), cls) = sampleCM(classes(i), cls) + 1;
end;

classCount=hist(classes,[1:10]);
fprintf(2,'Classifier accuracy per class:\n');
diag(sampleCM)./classCount'
fprintf(2,'Average correct classification rate.\n');
mean(diag(sampleCM)./classCount')
figure(2);clf;imagesc(sampleCM);axis image;colormap(jet);title('Confusion Matrix');

