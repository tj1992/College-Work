#!/bin/bash
# xkcd_get: xkcd comics downloader for offline reading

prog_name=$0
xkcd_dir=$HOME/Downloads/xkcd/
wget_args="--no-verbose -erobots=off --adjust-extension --span-hosts --exclude-domains google.com --convert-links --page-requisites"
tmpfile=/tmp/$prog_name.$$
xkcd_json_url="http://xkcd.com/info.0.json"

print_usage_exit() {
	printf "Usage: %s [-d -c -h]
		\n  -d              download the comics missing from www.xkcd.com/ directory
		\n  -t              check whether a new comic is available
		\n  -c              convert the links present in HTML files to point to local ones
		\n  -h              display this help and exit
		\n  -p [directory]  get/set the download directory path
		\nDownloads the xkcd.com comics for offline reading
		\n" $prog_name 1>&2;
	exit 0
}

# error reporter
error() {						# args: <msg> <command> <args-to-command>
	printf "$prog_name: %s" "$1"
	printf "\n*** log ***:\n"			# print the log file
	cat $tmpfile
	shift; eval $@					# evaluate the commands
	rm -f $tmpfile
	exit 1
}

# overwrite $1 with output from $2*
overwrite() {						# args: <filename> <command> <args-to-command>
	file=$1; shift					# save the filename and shift the args
	new=/tmp/overwr1.$$; old=/tmp/overwr.$$		# generate unique names ($$ returns current pid)
	trap 'error "overwrite trapped" rm -f $new $old' SIGHUP SIGINT SIGTERM # clean-up if trapped
	eval "$@" >$new					# run the commands and save output in a new file
	if test $? == 0					# successful execution ?
	then
		cp $file $old				# save old copy
		cp $new $file				# make new one
		if test $? != 0				# copy failed?
		then
			cp $old $file 			# restore old copy
			if test $? != 0			# check whether restoration failed
			then
				error "overwrite failed: $file corrupted" rm -f $new $old
			fi
		fi
	else
		error "overwrite: $1 failed, $file unchanged" rm -f $new
	fi
	rm -f $new $old					# clean up
}
					
# convert the links in $@ to local directories and strip the <script> tag
convert() {						# args: <files>
	trap 'error "convert trapped"' SIGHUP SIGINT SIGTERM
	for i in $@
	do
		overwrite $i "sed 's^\(http://www.xkcd.com/\)\([1-9][0-9]*\)/^\2.html^g
				   s^http://www.xkcd.com/archive^archive.html^g
				     s^\(http://\)\(imgs.xkcd.com\)^../\2^g
				     1,14s^http://www.xkcd.com/^^g' $i | awk 'BEGIN { pmat=0; }
									      /<script/,/<\/script>/ { pmat=1 }
									      pmat == 0 { print }
									      pmat == 1 { pmat=0 }'"
				     # convert the links to local ones and strip the <script> tag
	done
}

# create the log file and symlink to archive.html
create_log_link() {								# no args
	ls www.xkcd.com/ | sort -n | tail -n 1 | sed "s/[^0-9]//g"  >log	# save the latest comic # to "log" (alt: ls -c; if archive.html is updated before downloading the comics)
	
	if [ ! -e xkcd -o ! -z create_link ]; then							# create symlink to 'archive.html' if not present
		ln -sf www.xkcd.com/archive.html xkcd 1>&2 2>> $tmpfile		# redirect stdout to same stream as stderr and append to $tmpfile
		if test $? != 0; then error "ln failed!!"; fi
	fi
}

# get the latest comic #
get_latest() {
	latest_json=$(curl -s $xkcd_json_url)		# download the json file
	if [ -z "$latest_json" ]; then error "curl: download failed!"; fi
	latest=$(echo $latest_json | jq '.num')		# parse using jq
	if test $latest == "null"; then error "bad json format"; fi
}
	
# prepare the list of comics to be downloaded
prepare_list() {
	pages=""
	#if [ ! -e www.xkcd.com -a ! -e xkcd.com -a ! -e imgs.xkcd.com ]; then
	
	if [ ! -e log ]; then
		curr=1
		echo $curr >log							# "log" contains the latest comic #
	else
		curr=$(cat log)
	fi

	#latest=`curl -s xkcd.com | egrep -o 'http://xkcd.com/([1-9][0-9]*)' | sed 's/[^0-9]//g'`	# get the latest comic
	get_latest
	
	if [ $(($latest - $curr)) -gt 50 ]; then				# download in batches of size 50
		latest=$(($curr + 50))
		recur=1
	fi
	
	if [ $curr -ne $latest ]; then						# prepare the download list
		for ((i=$curr;i<=$latest;++i))
		do
			pages=$pages" www.xkcd.com/$i"
			files=$files" www.xkcd.com/${i}.html"
		done
	fi

	if [ ! -z "$pages" -o  ! -e www.xkcd.com/archive.html ]; then		# download the archive only if pages are to be downloaded or it is not present
		pages=$pages" www.xkcd.com/archive"
		files=$files" www.xkcd.com/archive.html"
		create_link=1
	fi

}

# ********************************************************************** START **********************************************************************

while getopts ":dtchp:" opt; do
	case $opt in
		d)
			dl_flag=1
			;;
		t)
			test_flag=1
			;;
		c)
			convert "$(ls www.xkcd.com/*.html)"
			exit 0
			;;
		h)
			print_usage_exit
			;;
		p)
			sed -r -i "s|^(xkcd_dir=)(.*)$|\1$OPTARG|" xkcd_get.sh
			exit 0
			;;
		\?)
			echo "$prog_name: Invalid option: -$OPTARG" 1>&2
			print_usage_exit
			;;
		:)
			if [ "$OPTARG" == "p" ]; then
				echo $xkcd_dir
				exit 0
			else
				echo "$prog_name: error: -$OPTARG: expected argument"
				exit 1
			fi
			;;

	esac
done

trap 'error "trapped"' SIGHUP SIGINT SIGTERM
printf "START: %s\n" "$(date)" >$tmpfile

if [ ! -e $xkcd_dir ]; then						# create download directory if not present
	if test "$test_flag" == 1; then error "$xkcd_dir does not exist. Exiting!!"; fi 	# don't create $xkcd_dir with $test_flag set
	mkdir $xkcd_dir 2>> $tmpfile
	if test $? != 0; then error "mkdir failed!!"; fi
fi
cd $xkcd_dir								# move to $xkcd_dir

if [ -z "$dl_flag" -o -n "$test_flag" ]; then
	prepare_list
else
	tempout=$(find www.xkcd.com -maxdepth 1 -type f | cut -c 14- | sort -n | awk -F '.' 'BEGIN { IN=1 } /[0-9][1-9]*/ { if ($1 != IN) { if (IN != 404) printf "www.xkcd.com/%d\twww.xkcd.com/%d.html\n", IN, IN; IN++ } IN++; }')	# get the missing comics (p.s. 404 is special case; comic #404 does not exist!)
	pages=$(echo "$tempout" | cut -f 1)				# pages to be downloaded
	files=$(echo "$tempout" | cut -f 2)				# files to be converted
fi

if [ -z "$pages" ]; then
	printf "Nothing to download!! exiting!!\n"
	exit 0
fi

if [ -n "$test_flag" ]; then
	printf "New Comic is available!\nDownload list: %s\n" "$(echo "$pages" | sed 's/[^0-9 ]//g')"
	exit 0
fi

wget $wget_args $pages 2>&1 | tee -a $tmpfile				# DOWNLOAD!
if test $? != 0; then error "wget failed!!"; fi

convert $files

create_log_link

rm $tmpfile

if [ ! -z $recur ]; then						# recursive call
	printf "\n*** Sleeping for 10 seconds... ***\n"
	sleep 10
	$prog_name
fi

# EOF
