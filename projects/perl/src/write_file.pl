#!/usr/bin/perl

#
#  打开一个文件并写入 "Hello world"
#

use strict;
use warnings;

my $logfile = "/tmp/perl.log";

open(my $file, '>', $logfile);

print $file "Hello world!\n";

close($file);
