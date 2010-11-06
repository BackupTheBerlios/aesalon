#!/usr/bin/env perl

sub openNamespace {
	print FILE  "namespace @_[1] {\n\n";
}

sub closeNamespace {
	print FILE "\n} // namespace @_[1]\n";
}

sub classDefinition {
	print FILE "class @_[1] {\n";
	print FILE "public:\n";
	print FILE "\t@_[1]();\n";
	print FILE "\t~@_[1]();\n";
	print FILE "};\n";
}

sub printCopyright {
	# Copyright notice . . .
	print FILE "/**\n";
	print FILE "    Aesalon, a tool to visualize a program's behaviour at run-time.\n";
	print FILE "    Copyright (C) 2010, Aesalon Development Team.\n\n";
	
	print FILE "    Aesalon is distributed under the terms of the GNU GPLv3. For more\n";
	print FILE "    licensing information, see the file LICENSE included with the distribution.\n\n";
	print FILE "    \@file @_[0]\n";
	print FILE "*/\n";
}

sub createHeader {
	$filename = @_[0] . (lc @_[1]) . "/" . @_[2] . ".h\n";

	open(FILE, ">$filename") or die("Can't open $filename . . .");

	printCopyright($filename);

	# Inclusion guard . . .
	$guard = "Aesalon" . @_[3] . "_" . @_[1] . @_[2];
	# Remove duplicate . . .
	$guard =~ s/@_[1]@_[1]/@_[1]/;
	print FILE "#ifndef $guard\n";
	print FILE "#define $guard\n\n";

	openNamespace(FILE, @_[3], @_[1]);

	classDefinition(FILE, @_[2]);

	closeNamespace(FILE, @_[3]);

	print FILE "\n#endif\n";

	close(FILE);
}

sub createSource {
	$filename = @_[0] . (lc @_[1]) . "/" . @_[2] . ".cpp\n";

	open(FILE, ">$filename") or die("Can't open $filename . . .");
	
	printCopyright($filename);
	
	if(@_[1] ne "") {
		print FILE "#include \"" . (lc @_[1]) . "/@_[2].h\"\n\n";
	}
	
	openNamespace(FILE, @_[3], @_[1]);

	print FILE "\n";

	closeNamespace(FILE, @_[3]);

	close(FILE);
}

sub extractNamespace {
	if(!(@_[0] =~ m/(.+)\:\:(.+)/)) { return "", @_[0]; }
	
#	print "1: $1, 2: $2\n";
	
	if($2 eq "") { return "", $1; }
	return $1, $2
}

$argumentCount = $#ARGV + 1;

if($argumentCount < 1) {
	die("Not enough arguments given");
}

$mode = "";

# Parse mode . . .
if($argumentCount == 1) {
	$mode = "common";
}
else {
	if(lc $ARGV[0] eq "monitor") { $mode = "monitor"; }
	elsif(lc $ARGV[0] eq "visualizer") { $mode = "visualizer"; }
	elsif(lc $ARGV[0] eq "common") { $mode = "common"; }
	else {
		die "Unknown mode";
	}
}

if($mode eq "monitor") {
	createHeader("include/monitor/", extractNamespace($ARGV[1]), "Monitor");
	createSource("monitor/src/", extractNamespace($ARGV[1]), "Monitor");
}
elsif($mode eq "visualizer") {

}
elsif($mode eq "common") {
	createHeader("include/common/", extractNamespace($ARGV[1]), "Common");
}
