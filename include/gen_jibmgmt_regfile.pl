#!/usr/bin/env perl

#         _\|/_
#         (o o)
# +----oOO-{_}-OOo-------------+
# |          _      ^..^       |
# |    _   _(_     ( oo )  )~  |
# |  _) /)(// (/     ,,  ,,    |
# |                2017-10-18  | 
# +----------------------------+
#

use strict;
use warnings;

open(FH, "<", "$ENV{ICPRO_DIR}/units/jib1_mgmt/source/rtl/verilog/jib1_mgmt_regfile.v") or die "Cant't open file: $!";

my %addr_map = ();

while (<FH>) {
    chomp;
    if (/localparam\s*RA_(\w+)\s*=\s*(\S+)\s*;/) {
        my ($addr, $name) = ($2, $1);
        $addr =~ s/32'h/0x/;
        $addr_map{$addr} = $name;
    }
}

my %addr_entry_size;
my $start_addr = "";
my $size = 1;
my $prefix = "";

my $maxlen = 0;

my @array_addr;
foreach my $addr (sort keys %addr_map) {
    $addr_map{$addr} =~ /^${prefix}_?(\d+)$/;

    if (length($addr_map{$addr}) > $maxlen) {
        $maxlen = length($addr_map{$addr});
    }

    if (defined($1) && ($1 == $size)) {
        $size = $size + 1;
        #print "$addr_map{$addr} $addr\n";
        push @array_addr, $addr;
    } else {
        ($prefix = $addr_map{$addr}) =~ s/_?\d+$//;
        if ($start_addr ne "") {
            $addr_entry_size{$start_addr} = $size;
        }
        $start_addr = $addr;
        $size = 1;
    }
}
$addr_entry_size{$start_addr} = $size;

delete @addr_map{@array_addr};

printf("\n#ifndef __JIBMGMT_REGFILE_H__");
printf("\n#define __JIBMGMT_REGFILE_H__\n");
printf("\n#include <stdint.h>\n\n");
printf("typedef struct jibmgmt_config_str {\n");

$maxlen += 4;
my $unused_index = 0;
my $prev_addr    = "0x0";
my $init         = 0;
foreach my $addr (sort keys %addr_map) {
    if ($init && hex($addr) != hex($prev_addr)) {
        printf("\tuint32_t %-${maxlen}s; // \@0x%08x - 0x%08x\n", "__unused_${unused_index}\[" . int((hex($addr)-hex($prev_addr))/4) . "\]", hex($prev_addr), hex($addr)-4);
        $unused_index++;
    }
    if ($addr_entry_size{$addr} == 1) {
        printf("\tuint32_t %-${maxlen}s; // \@%s\n", lc($addr_map{$addr}), $addr);
    } else {
        (my $name = $addr_map{$addr}) =~ s/_?\d+$//;
        printf("\tuint32_t %-${maxlen}s; // \@%s\n", lc(${name}) . "\[$addr_entry_size{$addr}\]", $addr);
    }
    $prev_addr = sprintf("0x%08x", hex($addr) + 4*$addr_entry_size{$addr});
    $init = 1;
}

#printf("\tuint32_t __pad[%d]\n", ((1<<19) - hex($prev_addr)) / 4);
printf("} jibmgmt_config_str_t;\n");
printf("\n#endif /* __JIBMGMT_REGFILE_H__ */\n");

close(FH);
