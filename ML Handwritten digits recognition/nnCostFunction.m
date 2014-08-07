function [J grad] = nnCostFunction(nn_params, ...
                                   input_layer_size, ...
                                   hidden_layer_size, ...
                                   num_labels, ...
                                   X, y, lambda)
%NNCOSTFUNCTION Implements the neural network cost function for a two layer
%neural network which performs classification
%   [J grad] = NNCOSTFUNCTON(nn_params, hidden_layer_size, num_labels, ...
%   X, y, lambda) computes the cost and gradient of the neural network. The
%   parameters for the neural network are "unrolled" into the vector
%   nn_params and need to be converted back into the weight matrices. 
% 
%   The returned parameter grad should be a "unrolled" vector of the
%   partial derivatives of the neural network.
%

% Reshape nn_params back into the parameters Theta1 and Theta2, the weight matrices
% for our 2 layer neural network
Theta1 = reshape(nn_params(1:hidden_layer_size * (input_layer_size + 1)), ...
                 hidden_layer_size, (input_layer_size + 1));

Theta2 = reshape(nn_params((1 + (hidden_layer_size * (input_layer_size + 1))):end), ...
                 num_labels, (hidden_layer_size + 1));

m = size(X, 1);
         
J = 0;
Theta1_grad = zeros(size(Theta1));
Theta2_grad = zeros(size(Theta2));

a_1 = [ones(m,1) X];	% m x input_layer_size+1(=s1+1)
z_2 = a_1*Theta1';	% m x s2
a_2 = [ones(m,1) sigmoid(z_2)];	% m x s2+1
a_3 = sigmoid(a_2*Theta2');	% m x s3

Y = zeros(size(a_3));		% m x s3
for t = 1:m, Y(t,:)=[zeros(1,y(t,:)-1) 1 zeros(1,num_labels-y(t,:))]; end;

delta_3 = a_3 - Y;	% m x s3
delta_2 = delta_3*Theta2.*sigmoidGradient([ones(m,1) z_2]);	% m x s2+1

Theta1_grad = 1./m .* (delta_2(:,2:end))'*a_1;	% s2 x s1+1
Theta1_grad(:,2:end) += lambda./m .* Theta1(:,2:end);

Theta2_grad = 1./m .* delta_3'*a_2;	% s3 x s2+1
Theta2_grad(:,2:end) += lambda./m.*Theta2(:,2:end);

J = 1./m.*(sum(sum((-Y.*log(a_3)-(1-Y).*log(1-a_3)), 2)) + lambda./2.*(sum(sum(Theta1(:,2:end).^2))+sum(sum(Theta2(:,2:end).^2))));

% Unroll gradients
grad = [Theta1_grad(:) ; Theta2_grad(:)];

end
