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

sub createHeader {
	$filename = @_[0] . (lc @_[1]) . "/" . @_[2] . ".h\n";
#	print "$filename";
#	print "namespace: @_[3]\n";

	open(FILE, ">$filename") or die("Can't open $filename . . .");

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

sub extractNamespace {
	@_[0]  =~ m/(.+)\:\:(.+)/;
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
	if($ARGV[0] eq "Monitor") { $mode = "monitor"; }
	elsif($ARGV[0] eq "Visualizer") { $mode = "visualizer"; }
	else {
		die "Unknown mode";
	}
}

if($mode eq "monitor") {
	createHeader("monitor/src/", extractNamespace($ARGV[1]), "Monitor");
}
