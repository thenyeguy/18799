#! /usr/bin/perl
use warnings;
use strict;

#Tool used to read in text file from a path and output a text file 
#without punctuation or upper case letters



if(scalar(@ARGV)<1){
	print_help();
	exit(0);
}

my $text_file_path = $ARGV[0];

open(INFILE,$text_file_path) or die "Could not open file: $text_file_path\n";

my @text_array;
while(<INFILE>){
	my $line = $_;
	$line=~s/([^a-zA-Z_0-9 ])//g;
	#print "$_\n";
	#print "$line\n\n";
	my @words_on_line = split(/\s+/,$line);
	push(@text_array,@words_on_line);
}
=cut
for( my $ii = 0; $ii <scalar(@text_array); $ii++){
	print "$text_array[$ii]\n";
}
=cut
write_out_text(\@text_array,"output.txt");

close(INFILE);



sub print_help{
	print "Tool used to read in text file from a path\n";
	print "and output a text file without punctuation\n";
	print " or upper case letters\n";
	print "\nRequires Command Line text filepath\n\n";
}

sub write_out_text{
	my $text_array_ref = shift;
	my $out_file_path = shift;
	open(OUTFILE,">$out_file_path");
       	print OUTFILE join(' ',@{$text_array_ref});
	close(OUTFILE);
}


