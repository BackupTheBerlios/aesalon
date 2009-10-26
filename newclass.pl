#!/usr/bin/perl
# Perl script by DWK to create a new .cpp/.h combination template

if(scalar(@ARGV) < 1) {
    die "Usage: $0 <C++-name-of-class>\n" .
        "\te.g. $0 Engine::TerrainCoordinate\n";
}

process_class($ARGV[0]);

sub process_class {
    my $class = shift;
    my $prefix = "src/";
    
    while($class =~ /([^:]+)::/g) {
        $prefix .= lc($1) . '/';
    }
    
    $class =~ /([^:]+)$/;
    $prefix .= $1;
    
    print "Class prefix: \"$prefix\"\n";
    create_cpp_file($class, $prefix . ".cpp");
    create_h_file($class, $prefix . ".h");
}

sub create_cpp_file {
    my $class = shift;
    my $file = shift;
    open(FILE, ">$file") or die;
    
    $class =~ /([^:]+)$/;
    print FILE "#include \"$1.h\"\n";
    print FILE "\n";
    print FILE open_namespaces(generate_namespaces($class));
    print FILE "\n";
    print FILE close_namespaces(generate_namespaces($class));
    
    close FILE;
}

sub create_h_file {
    my $class = shift;
    my $file = shift;
    open(FILE, ">$file") or die;
    
    my $guard = $class;
    $guard =~ s/([A-Z]+)/substr($1, 0, 1) . lc(substr($1, 1))/eg;
    $guard =~ s/:://g;
    $guard =~ s/([A-Z])/'_' . lc($1)/eg;
    $guard = uc($guard);
    print FILE "#ifndef AESALON$guard" . "_H\n";
    print FILE "#define AESALON$guard" . "_H\n";
    print FILE "\n";
    print FILE open_namespaces(generate_namespaces($class));
    print FILE "\n";
    $class =~ /([^:]+)$/;
    print FILE "class $1 {\n";
    print FILE "public:\n";
    print FILE "};\n";
    print FILE "\n";
    print FILE close_namespaces(generate_namespaces($class));
    print FILE "\n";
    print FILE "#endif\n";
    
    close FILE;
}

sub generate_namespaces {
    my $class = shift;
    my @spaces = ('Aesalon');
    
    while($class =~ /([^:]+)::/g) {
        push(@spaces, $1);
    }
    
    return @spaces;
}

sub open_namespaces {
    my $return = '';
    for my $namespace (@_) {
        $return .= "namespace $namespace {\n";
    }
    
    return $return;
}

sub close_namespaces {
    my $return = '';
    for my $namespace (reverse @_) {
        $return .= "}  // namespace $namespace\n";
    }
    
    return $return;
}
