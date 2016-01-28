#!/usr/bin/env perl -w
#
# Copyright (c) 2004      The GLib Development Team.
# Copyright (c) 2005-2006 Benedikt Meurer <benny@xfce.org>.
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free
# Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
# Boston, MA 02110-1301 USA
#

my $option_def = 0;

if (($#ARGV >= 0) && ($ARGV[0] eq "-def"))
  {
    shift;
    $option_def = 1;
  }

print <<EOF;
/* Generated by make-libxfce4util-alias.pl. Do not edit this file. */

#ifdef HAVE_GNUC_VISIBILITY

#include <glib.h>

EOF

if ($option_def)
  {
    print <<EOF
#undef IN_HEADER
#define IN_HEADER(x) 1

#undef IN_SOURCE
#define IN_SOURCE defined

EOF
  }
else
  {
    print <<EOF
#define IN_HEADER defined
#define IN_SOURCE(x) 1

EOF
  }

my $in_comment = 0;
my $in_skipped_section = 0;

while (<>)
  {
    # ignore empty lines
    next if /^\s*$/;

    # skip comments
    if ($_ =~ /^\s*\/\*/)
      {
        $in_comment = 1;
      }
    
    if ($in_comment)
      {
        if ($_ =~  /\*\/\s$/)
          {
            $in_comment = 0;
          }
        next;
      }

    # handle ifdefs
    if ($_ =~ /^\#endif/)
      {
        if (!$in_skipped_section)
          {
            print $_;
          }

        $in_skipped_section = 0;
        next;
      }

    if ($_ =~ /^\#ifdef\s+(INCLUDE_VARIABLES|INCLUDE_INTERNAL_SYMBOLS|ALL_FILES)/)
      {
        $in_skipped_section = 1;
      }

    if ($in_skipped_section)
      {
        next;
      }

    if ($_ =~ /^\#ifn?def\s+G/)
      {
        print $_;
        next;
      }
   
    if ($_ =~ /^\#if.*(IN_SOURCE|IN_HEADER)/)
      {
        print $_;
        next;
      }

    chop;
    my $line = $_;
    my @words;
    my $attributes = "";

    @words = split (/ /, $line);
    my $symbol = shift (@words);
    chomp ($symbol);
    my $alias = "IA__".$symbol;
    
    # Drop any Win32 specific .def file syntax,  but keep attributes
    foreach $word (@words)
      {
        $attributes = "$attributes $word" unless $word eq "PRIVATE";
      }
    
    if (!$option_def)
      {
        print <<EOF
extern __typeof ($symbol) $alias __attribute((visibility("hidden")))$attributes;
\#define $symbol $alias

EOF
      }
    else
      {
        print <<EOF
\#undef $symbol 
extern __typeof ($symbol) $symbol __attribute((alias("$alias"), visibility("default")));

EOF
      }
  }

print <<EOF;

#endif /* HAVE_GNUC_VISIBILITY */
EOF


