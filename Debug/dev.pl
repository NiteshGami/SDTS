#!/usr/bin/perl
my @devices = split('\n\n', `usb-devices`);
my @samdevs = ();
while(($_ = shift @devices)&& $_ =~/manufacturer=samsung/i){
    push (@samdevs, $1) if m/serialnumber=([^\n]+)\n/i;
}

open(OUT, ">", "final.info");
print OUT "@samdevs\n";
