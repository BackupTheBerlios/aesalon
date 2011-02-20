#!/usr/bin/perl

my $file = $ARGV[0];

my $class = $file;
$class =~ s/\.h//;
$class =~ s|/|::|;
$class =~ s|::(.)|'::'.uc($1)|e;
$class =~ s|[Vv]comm|VComm|;
$class =~ s|shm|SHM|i;

#print "$class\n";

open(FILE, "<$file") or die;

while(<FILE>) {
    if(/\@file/) {
        print `/tmp/newsource.py cppheader $class | grep '\@file'`;
	}
    elsif(/^#define Aesalon/) {
        print `/tmp/newsource.py cppheader $class | grep define`;
	}
	elsif(/^#ifndef Aesalon/) {
        print `/tmp/newsource.py cppheader $class | grep ifndef`;
	}
	elsif(/^namespace /) {
        print `/tmp/newsource.py cppheader $class | grep ^namespace` if !$ns;
	    $ns = 1;
	}
	elsif(m|^} //|) {
        print `/tmp/newsource.py cppheader $class | grep '^} //'` if !$close;
		$close = 1;
	}
	else {
        print;
	}
}
