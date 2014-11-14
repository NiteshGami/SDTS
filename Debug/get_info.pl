#!/usr/bin/perl
@sdbout = split('\n', `which sdb`);
$sdb = shift(@sdbout);
$out = `$sdb devices` or warn "No sdb";
@array = split('\n', $out);
my $listof = shift(@array);
unless ($listof =~ m/^List/) {
	$sdb = "~/tizen-sdk/tools/sdb";
	$out = `$sdb devices`;
	@array = split('\n', $out);
	my $listof = shift(@array);
	print "$listof\n";
	die "Install sdb properly, contact system team" unless $listof =~ /^List/;
}

while (my $var = shift(@array)) {
	my @line = split('\s', $var);
	my $device = shift(@line);
	print "$device\n";
	$cmd = "$sdb -s $device";
	`$cmd root on; $cmd pull /opt/etc/.target.info .` or warn "$!";
	`cat .target.info >> final.info` or warn "$!";
}
