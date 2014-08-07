function [y, msg] = load_labels(fname)
% LOAD_LABELS Load label data from IDX file.
%	[y, msg] = LOAD_LABELS(fname) loads the labels present in
%	IDX file "fname". If "fname" can't be opened or any other
%	IO error occurs, appropriate error message is set in "msg"
%	and "y" is set to '[]'.

y = [];

[fid, msg] = fopen(fname, 'r');
if fid == -1
	return
end

header = fread(fid, 4, 'uint8');
if bsxfun(@ne, header(1:2), uint8([0 0]))
	msg = 'Invalid file type'
	return
elseif header(3) != 8
	msg = 'Handling of non uint8 types has not been implemented yet';
	return
elseif header(4) != 1
	msg = 'Multi-dimensional data not supported!';
	return
end

dim = fread(fid, 1, 'uint32', 'ieee-be');
y = fread(fid, dim, 'uint8');

% replace 0 with 10 for easier access while indexing
y(y == 0) = 10;

fclose(fid);

end
