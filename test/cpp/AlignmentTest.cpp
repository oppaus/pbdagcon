#include <iostream>
#include <fstream>
#include <string>
#include <gtest/gtest.h>
#include <boost/graph/adjacency_list.hpp>
#include "Alignment.hpp"

using namespace dagcon;

TEST(Alignment, Normalize) {
    Alignment a, b;
    a.start = 1;

    // test mismatch
    b.qstr = "CAC";
    b.tstr = "CGC";
    b = normalizeGaps(b);
    EXPECT_EQ("C-AC", b.qstr);
    EXPECT_EQ("CG-C", b.tstr);

    // test shifting gaps on read
    b.qstr = "-C--CGT";
    b.tstr = "CCGAC-T";
    b = normalizeGaps(b);
    EXPECT_EQ("CCG--T", b.qstr);
    EXPECT_EQ("CCGACT", b.tstr);
    
    // another gap reference push
    a.tstr = "ATATTA---GGC"; 
    a.qstr = "ATAT-AGCCGGC"; 

    b = a;
    b = normalizeGaps(b);
    // query remains unchanged
    EXPECT_EQ(a.qstr, b.qstr);
    // target shifts the G bases to the left
    EXPECT_EQ("ATATTAG--G-C", b.tstr);
}

TEST(Alignment, ParseBasic) {
    std::ifstream file("basic.m5");
    Alignment aln;
    file >> aln;
    EXPECT_EQ(1, aln.start);
    EXPECT_EQ("CAC", aln.qstr);
    EXPECT_EQ("CGC", aln.tstr);

    file >> aln;
    EXPECT_EQ(1, aln.start);
    EXPECT_EQ("AATTGGCC", aln.qstr);
    EXPECT_EQ("GGCCAATT", aln.tstr);
}

TEST(Alignment, ParseQuery) {
    std::ifstream file("parsequery.m5");
    Alignment aln;
    Alignment::groupByTarget = false;
    file >> aln;
    EXPECT_EQ("m130110_062238_00114_c100480560100000001823075906281381_s1_p0/311/1102_3151",
              aln.id);
    EXPECT_EQ(2049, aln.len);
    EXPECT_EQ(8, aln.start);
    EXPECT_EQ("CTGCATGCT", aln.tstr.substr(0,9));
    EXPECT_EQ("CTGCA--CT", aln.qstr.substr(0,9));
}
