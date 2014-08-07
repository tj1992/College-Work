function [X, msg] = load_images(fname)
% LOAD_IMAGES Load images data from IDX file.
%	[X, msg] = LOAD_IMAGES(fname) loads the images present in
%	IDX file "fname". If "fname" can't be opened or any other
%	IO error occurs, appropriate error message is set in "msg"
%	and "X" is set to '[]'.

X = [];
[fid, msg] = fopen(fname, 'r');
if fid == -1
	return
end

header = fread(fid, 4, 'uint8');
if bsxfun(@ne, header(1:2), uint8([0 0]))
	msg = 'Invalid file type';
	return
elseif header[3] != 8
	msg = 'Handling of non uint8 types has not been implemented yet';
	return
end

dim = uint32(zeros(header(4), 1));
for i = 1:length(dim),
	dim(i) = fread(fid, 1, 'uint32', 'ieee-be');
end

X = fread(fid, [prod(dim(2:end)) dim(1)], 'uint8')';

fclose(fid);

end


