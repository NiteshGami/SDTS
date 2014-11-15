#!/usr/bin/perl
my @devices = split('\n\n', `usb-devices`);
my @samdevs = ();
while($_ = shift @devices) {
	if ($_ =~ m/manufacturer\=samsung/i){
    		push (@samdevs, $1) if ($_ =~ m/serialnumber\=(\S+)/i);
	}
}
print "@samdevs\n";
open(OUT, ">", "final.info");
print OUT "@samdevs\n";
