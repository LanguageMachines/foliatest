/*
  Copyright (c) 2006 - 2019
  CLST  - Radboud University
  ILK   - Tilburg University

  This file is part of foliatest

  foliatest is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.

  foliatest is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

  For questions and suggestions, see:
      https://github.com/LanguageMachines/foliatest/issues
  or send mail to:
      lamasoftware (at ) science.ru.nl

*/

#include <cstdlib>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <list>
#include <vector>
#include <map>
#include <algorithm>
#include <stdexcept>
#include <unicode/unistr.h>
#include "libxml/tree.h"
#include "ticcutils/StringOps.h"
#include "ticcutils/PrettyPrint.h"
#include "ticcutils/XMLtools.h"
#include "libfolia/folia.h"
#include "ticcutils/UnitTest.h"

#include "config.h"
using namespace std;
using namespace icu;
using namespace folia;

using TiCC::operator<<;

#if FOLIA_INT_VERSION < 116
#define XML_ID "id"
#else
#define XML_ID "xml:id"
#endif
#if FOLIA_INT_VERSION > 116
#define AlignReference LinkReference
#define Alignment Relation
#define ComplexAlignment SpanRelation
#define ComplexAlignmentLayer SpanRelationLayer
#define AbstractTokenAnnotation AbstractInlineAnnotation
#define AbstractTokenAnnotation_t AbstractInlineAnnotation_t
#endif

void test0() {
  startTestSerie( " Test lezen van KWargs " );
  KWargs bla;
#if FOLIA_INT_VERSION < 116
  assertNoThrow( bla = getArgs( "dit='goed', dat =' ra ar' " ) );
#else
  assertNoThrow( bla.init( "dit='goed', dat =' ra ar' " ) );
#endif
  assertTrue( bla["dit"] == "goed" );
  assertTrue( bla["dat"] == " ra ar" );
  assertEqual ( bla["dat"] , string(" ra ar") );
  assertThrow( bla = getArgs( "dit='fout' = " ), ArgsError );
  assertThrow( bla = getArgs( "dit=fout" ), ArgsError );
  assertNoThrow( bla = getArgs( "cls='o\\'k'" ) );
  assertTrue( bla["cls"] == "o'k" );
  assertNoThrow( bla = getArgs( "class='o\\k'" ) );
  assertTrue( bla["class"] == "o\\k" );
  assertNoThrow( bla = getArgs( "cls='o\"k'" ) );
  assertTrue( bla["cls"] == "o\"k" );
  assertNoThrow( bla = getArgs( "cls='o""k'" ) );
  assertTrue( bla["cls"] == "ok" );
#if FOLIA_INT_VERSION < 116
  assertNoThrow( bla = getArgs( "class='ok\\a', bli='bla'" ) );
  assertTrue( bla["class"] == "ok\\a" );
#else
  assertNoThrow( bla.init( "class='ok\\a', bli='bla'" ) );
  assertEqual( bla.is_present( "class" ), true );
  assertEqual( bla.is_present( "klas" ), false );
  assertEqual( bla.extract("class"), "ok\\a" );
  assertEqual( bla.extract("class"), "" );
  assertEqual( bla.extract("klas"), "" );
#endif
  assertTrue( bla["bli"] == "bla" );
  assertNoThrow( bla = getArgs( "cls='ok\\\\', bli='bla'" ) );
  assertTrue( bla["cls"] == "ok\\" );
  assertTrue( bla["bli"] == "bla" );
}


void test1() {
  startTestSerie( " Test lezen van een FoLiA file " );
  Document d;
  assertNoThrow( d.readFromFile( "tests/example.xml" ) );
  assertNoThrow( d.save( "/tmp/example.xml" ) );
  int stat = system( "./tests/foliadiff.sh /tmp/example.xml tests/example.xml" );
  assertMessage( "/tmp/example.xml tests/example.xml differ!",
   		 (stat == 0) );
}

void test1a() {
  startTestSerie( " Test lezen en schrijven van een BZ2 FoLiA file " );
  Document d1;
  assertNoThrow( d1.readFromFile( "tests/example.xml" ) );
  assertNoThrow( d1.save( "/tmp/example.xml.bz2" ) );
  Document d2;
  assertNoThrow( d2.readFromFile( "/tmp/example.xml.bz2" ) );
  assertNoThrow( d2.save( "/tmp/example.xml" ) );
  int stat = system( "./tests/foliadiff.sh /tmp/example.xml tests/example.xml" );
  assertMessage( "/tmp/example.xml tests/example.xml differ!",
   		 (stat == 0) );
}

void test1b() {
  startTestSerie( " Test lezen en schrijven van een GZ FoLiA file " );
  Document d1;
  assertNoThrow( d1.readFromFile( "tests/example.xml" ) );
  assertNoThrow( d1.save( "/tmp/example.xml.gz" ) );
  Document d2;
  assertNoThrow( d2.readFromFile( "/tmp/example.xml.gz" ) );
  assertNoThrow( d2.save( "/tmp/folia.gz.example" ) );
  int stat = system( "./tests/foliadiff.sh /tmp/folia.gz.example tests/example.xml" );
  assertMessage( "/tmp/folia.gz.example tests/example.xml differ!",
   		 (stat == 0) );
}

void test1c() {
  startTestSerie( " Test lezen van een FoLiA speech file " );
  Document d;
  assertNoThrow( d.readFromFile( "tests/speechexample.xml" ) );
  assertNoThrow( d.save( "/tmp/speechexample.xml" ) );
  int stat = system( "./tests/foliadiff.sh /tmp/speechexample.xml tests/speechexample.xml" );
  assertMessage( "/tmp/speechexample.xml tests/speechexample.xml differ!",
   		 (stat == 0) );
}

void test1d() {
  startTestSerie( " Test lezen van een FoLiA invalid file " );
  Document d;
  assertThrow( d.readFromFile( "tests/unknowntag.xml" ), XmlError );
}

#if FOLIA_INT_VERSION >= 116
void test1e() {
  startTestSerie( " Test lezen en schrijven van een lastige FoLiA file " );
  Document d1;
  assertThrow( d1.readFromFile( "tests/scary.xml" ), XmlError );
}
#elif FOLIA_INT_VERSION >= 115
void test1e() {
  startTestSerie( " Test lezen en schrijven van een lastige FoLiA file " );
  Document d1;
  assertNoThrow( d1.readFromFile( "tests/scary.xml" ) );
  assertNoThrow( d1.save( "/tmp/scary.xml" ) );
  vector<Sentence*> sv;
  assertNoThrow( sv = d1.sentences() );
  assertEqual( sv[0]->text(), "Is@" );
  Document d2;
  assertNoThrow( d2.readFromFile( "/tmp/scary.xml" ) );
  assertNoThrow( d2.save( "/tmp/scary2.xml" ) );
  int stat = system( "./tests/foliadiff.sh /tmp/scary.xml /tmp/scary2.xml" );
  assertMessage( "/tmp/scary.xml /tmp/scary2.xml differ!",
   		 (stat == 0) );
}
#endif
#if FOLIA_INT_VERSION >= 114
void test1f() {
  startTestSerie( " Test lezen en schrijven van een FoLiA file met entities" );
  Document d1;
  assertNoThrow( d1.readFromFile( "tests/entities.xml" ) );
  Sentence *s = d1.sentences()[0];
  assertEqual( s->text(), "Dit <is> als het ware één test met ß." );
  assertNoThrow( d1.save( "/tmp/entities.xml" ) );
  int stat = system( "./tests/foliadiff.sh /tmp/entities.xml tests/entities.xml" );
  assertMessage( "/tmp/entities.xml tests/entities.xml differ!",
   		 (stat == 0) );
}
#endif

void test2() {
  startTestSerie( " Test lezen van een FoLiA string " );
  string s;
  ifstream is( "tests/example.xml" );
  string line;
  while( getline( is, line ) ){
    s += line + "\n";
  };
  Document d;
  assertNoThrow( d.readFromString( s ) );
}

void test3() {
  startTestSerie( " Test lezen van een DCOI file " );
  Document d;
  assertThrow( d.readFromFile( "tests/dcoi.example" ), XmlError );
}

void test4() {
  startTestSerie( " Test uitvoer van een FoLiA file naar string" );
  Document d;
  assertNoThrow( d.readFromFile( "tests/example.xml" ) );
  string out;
  assertNoThrow( out = d.toXml() );
}

void test5() {
  startTestSerie( " Test lezen van een FoLiA file met namespaces" );
  Document d;
  assertNoThrow( d.readFromFile( "tests/folia.nsexample" ) );
  assertNoThrow( d.save( "/tmp/test5.out", "fl", false ) );
  int stat = system( "./tests/foliadiff.sh /tmp/test5.out tests/folia.nsexample" );
  assertMessage( "/tmp/test5.out tests/folia.nsexample differ!",
   		 (stat == 0) );
}

void test6() {
  startTestSerie( " Test lezen van een FoLiA file external nodes" );
  Document d;
  assertNoThrow( d.readFromFile( "tests/include1.xml" ) );
  assertNoThrow( d.save( "/tmp/include.out" ) );
  int stat = system( "./tests/foliadiff.sh /tmp/include.out tests/include.ok" );
  assertMessage( "/tmp/include.out tests/include.ok differ!",
   		 (stat == 0) );
  Document d2;
  assertThrow( d2.readFromFile( "tests/include2.xml" ), XmlError );
}

void test7() {
  startTestSerie( " Test inlezen van een FoLiA file zonder namespace declaratie" );
  Document d;
  assertThrow( d.readFromFile( "tests/noname.xml" ), XmlError );
}

void test8() {
  startTestSerie( " Test inlezen van een FoLiA file met foute namespace declaratie" );
  Document d;
  assertThrow( d.readFromFile( "tests/wrongname.xml" ), XmlError );
}

void test9() {
  startTestSerie( " Test extracting text from a document" );
  Document doc;
  assertNoThrow( doc.readFromFile( "tests/text.xml" ) );
  UnicodeString us;
  assertNoThrow( us = doc.text() );
  assertEqual( us, "chapter 1\n\nsentence 1" );
}

Document sanityDoc( "file='tests/example.xml'" );

void sanity_test000( ){
  startTestSerie( " Text count " );
  assertEqual( sanityDoc.size(), 1 );
  assertTrue( sanityDoc[0]->isinstance<Text>() );

}

void sanity_test001( ){
  startTestSerie(" Paragraph count " );
  assertEqual( sanityDoc.paragraphs().size(), 3 );

}

void sanity_test002(){
  startTestSerie(" Sentence count " );
  vector<Sentence*> v;
  assertNoThrow( v = sanityDoc.sentences() );
  assertEqual( 17, v.size() );

}

void sanity_test003a( ){
  startTestSerie(" Word count " );
  vector<Word*> v;
  assertNoThrow( v = sanityDoc.words() );
  assertEqual( 190, v.size() );

}

void sanity_test003b( ){
  startTestSerie(" Word iterator " );
  vector<Word*> v;
  assertNoThrow( v = sanityDoc.words() );
  string result;
  for ( const auto& a: v ){
    result += "'" + a->id() + "', ";
  }
  assertEqual( result, "'WR-P-E-J-0000000001.head.1.s.1.w.1', 'WR-P-E-J-0000000001.p.1.s.1.w.1', 'WR-P-E-J-0000000001.p.1.s.1.w.2', 'WR-P-E-J-0000000001.p.1.s.1.w.3', 'WR-P-E-J-0000000001.p.1.s.1.w.4', 'WR-P-E-J-0000000001.p.1.s.1.w.5', 'WR-P-E-J-0000000001.p.1.s.1.w.6', 'WR-P-E-J-0000000001.p.1.s.1.w.7', 'WR-P-E-J-0000000001.p.1.s.1.w.8', 'WR-P-E-J-0000000001.p.1.s.2.w.1', 'WR-P-E-J-0000000001.p.1.s.2.w.2', 'WR-P-E-J-0000000001.p.1.s.2.w.3', 'WR-P-E-J-0000000001.p.1.s.2.w.4', 'WR-P-E-J-0000000001.p.1.s.2.w.5', 'WR-P-E-J-0000000001.p.1.s.2.w.6', 'WR-P-E-J-0000000001.p.1.s.2.w.7', 'WR-P-E-J-0000000001.p.1.s.2.w.8', 'WR-P-E-J-0000000001.p.1.s.2.w.9', 'WR-P-E-J-0000000001.p.1.s.2.w.10', 'WR-P-E-J-0000000001.p.1.s.2.w.11', 'WR-P-E-J-0000000001.p.1.s.2.w.12', 'WR-P-E-J-0000000001.p.1.s.2.w.13', 'WR-P-E-J-0000000001.p.1.s.2.w.14', 'WR-P-E-J-0000000001.p.1.s.2.w.15', 'WR-P-E-J-0000000001.p.1.s.2.w.16', 'WR-P-E-J-0000000001.p.1.s.2.w.17', 'WR-P-E-J-0000000001.p.1.s.2.w.18', 'WR-P-E-J-0000000001.p.1.s.2.w.19', 'WR-P-E-J-0000000001.p.1.s.2.w.20', 'WR-P-E-J-0000000001.p.1.s.2.w.21', 'WR-P-E-J-0000000001.p.1.s.2.w.22', 'WR-P-E-J-0000000001.p.1.s.2.w.23', 'WR-P-E-J-0000000001.p.1.s.2.w.24-25', 'WR-P-E-J-0000000001.p.1.s.2.w.26', 'WR-P-E-J-0000000001.p.1.s.2.w.27', 'WR-P-E-J-0000000001.p.1.s.2.w.28', 'WR-P-E-J-0000000001.p.1.s.2.w.29', 'WR-P-E-J-0000000001.p.1.s.3.w.1', 'WR-P-E-J-0000000001.p.1.s.3.w.2', 'WR-P-E-J-0000000001.p.1.s.3.w.3', 'WR-P-E-J-0000000001.p.1.s.3.w.4', 'WR-P-E-J-0000000001.p.1.s.3.w.5', 'WR-P-E-J-0000000001.p.1.s.3.w.6', 'WR-P-E-J-0000000001.p.1.s.3.w.7', 'WR-P-E-J-0000000001.p.1.s.3.w.8', 'WR-P-E-J-0000000001.p.1.s.3.w.9', 'WR-P-E-J-0000000001.p.1.s.3.w.10', 'WR-P-E-J-0000000001.p.1.s.3.w.11', 'WR-P-E-J-0000000001.p.1.s.3.w.12', 'WR-P-E-J-0000000001.p.1.s.3.w.13', 'WR-P-E-J-0000000001.p.1.s.3.w.14', 'WR-P-E-J-0000000001.p.1.s.3.w.15', 'WR-P-E-J-0000000001.p.1.s.3.w.16', 'WR-P-E-J-0000000001.p.1.s.3.w.17', 'WR-P-E-J-0000000001.p.1.s.3.w.18', 'WR-P-E-J-0000000001.p.1.s.3.w.19', 'WR-P-E-J-0000000001.p.1.s.3.w.20', 'WR-P-E-J-0000000001.p.1.s.3.w.21', 'WR-P-E-J-0000000001.p.1.s.4.w.1', 'WR-P-E-J-0000000001.p.1.s.4.w.2', 'WR-P-E-J-0000000001.p.1.s.4.w.3', 'WR-P-E-J-0000000001.p.1.s.4.w.4', 'WR-P-E-J-0000000001.p.1.s.4.w.5', 'WR-P-E-J-0000000001.p.1.s.4.w.6', 'WR-P-E-J-0000000001.p.1.s.4.w.7', 'WR-P-E-J-0000000001.p.1.s.4.w.8', 'WR-P-E-J-0000000001.p.1.s.4.w.9', 'WR-P-E-J-0000000001.p.1.s.4.w.10', 'WR-P-E-J-0000000001.p.1.s.5.w.1', 'WR-P-E-J-0000000001.p.1.s.5.w.2', 'WR-P-E-J-0000000001.p.1.s.5.w.3', 'WR-P-E-J-0000000001.p.1.s.5.w.4', 'WR-P-E-J-0000000001.p.1.s.5.w.5', 'WR-P-E-J-0000000001.p.1.s.5.w.6', 'WR-P-E-J-0000000001.p.1.s.5.w.7', 'WR-P-E-J-0000000001.p.1.s.5.w.8', 'WR-P-E-J-0000000001.p.1.s.5.w.9', 'WR-P-E-J-0000000001.p.1.s.5.w.10', 'WR-P-E-J-0000000001.p.1.s.5.w.11', 'WR-P-E-J-0000000001.p.1.s.5.w.12', 'WR-P-E-J-0000000001.p.1.s.5.w.13', 'WR-P-E-J-0000000001.p.1.s.5.w.14', 'WR-P-E-J-0000000001.p.1.s.5.w.15', 'WR-P-E-J-0000000001.p.1.s.5.w.16', 'WR-P-E-J-0000000001.p.1.s.5.w.17', 'WR-P-E-J-0000000001.p.1.s.5.w.18', 'WR-P-E-J-0000000001.p.1.s.5.w.19', 'WR-P-E-J-0000000001.p.1.s.5.w.20', 'WR-P-E-J-0000000001.p.1.s.5.w.21', 'WR-P-E-J-0000000001.p.1.s.6.w.1', 'WR-P-E-J-0000000001.p.1.s.6.w.2', 'WR-P-E-J-0000000001.p.1.s.6.w.3', 'WR-P-E-J-0000000001.p.1.s.6.w.4', 'WR-P-E-J-0000000001.p.1.s.6.w.5', 'WR-P-E-J-0000000001.p.1.s.6.w.6', 'WR-P-E-J-0000000001.p.1.s.6.w.7', 'WR-P-E-J-0000000001.p.1.s.6.w.8', 'WR-P-E-J-0000000001.p.1.s.6.w.9', 'WR-P-E-J-0000000001.p.1.s.6.w.10', 'WR-P-E-J-0000000001.p.1.s.6.w.11', 'WR-P-E-J-0000000001.p.1.s.6.w.12', 'WR-P-E-J-0000000001.p.1.s.6.w.13', 'WR-P-E-J-0000000001.p.1.s.6.w.14', 'WR-P-E-J-0000000001.p.1.s.6.w.15', 'WR-P-E-J-0000000001.p.1.s.6.w.16', 'WR-P-E-J-0000000001.p.1.s.6.w.17', 'WR-P-E-J-0000000001.p.1.s.6.w.18', 'WR-P-E-J-0000000001.p.1.s.6.w.19', 'WR-P-E-J-0000000001.p.1.s.6.w.20', 'WR-P-E-J-0000000001.p.1.s.6.w.21', 'WR-P-E-J-0000000001.p.1.s.6.w.22', 'WR-P-E-J-0000000001.p.1.s.6.w.23', 'WR-P-E-J-0000000001.p.1.s.6.w.24', 'WR-P-E-J-0000000001.p.1.s.6.w.25', 'WR-P-E-J-0000000001.p.1.s.6.w.26', 'WR-P-E-J-0000000001.p.1.s.6.w.27', 'WR-P-E-J-0000000001.p.1.s.6.w.28', 'WR-P-E-J-0000000001.p.1.s.6.w.29', 'WR-P-E-J-0000000001.p.1.s.6.w.30', 'WR-P-E-J-0000000001.p.1.s.6.w.31', 'WR-P-E-J-0000000001.p.1.s.6.w.32', 'WR-P-E-J-0000000001.p.1.s.6.w.33', 'WR-P-E-J-0000000001.p.1.s.6.w.34', 'WR-P-E-J-0000000001.p.1.s.7.w.1', 'WR-P-E-J-0000000001.p.1.s.7.w.2', 'WR-P-E-J-0000000001.p.1.s.7.w.3', 'WR-P-E-J-0000000001.p.1.s.7.w.4', 'WR-P-E-J-0000000001.p.1.s.7.w.5', 'WR-P-E-J-0000000001.p.1.s.7.w.6', 'WR-P-E-J-0000000001.p.1.s.7.w.7', 'WR-P-E-J-0000000001.p.1.s.7.w.8', 'WR-P-E-J-0000000001.p.1.s.7.w.9', 'WR-P-E-J-0000000001.p.1.s.7.w.10', 'WR-P-E-J-0000000001.p.1.s.8.w.1', 'WR-P-E-J-0000000001.p.1.s.8.w.2', 'WR-P-E-J-0000000001.p.1.s.8.w.3', 'WR-P-E-J-0000000001.p.1.s.8.w.4', 'WR-P-E-J-0000000001.p.1.s.8.w.5', 'WR-P-E-J-0000000001.p.1.s.8.w.6', 'WR-P-E-J-0000000001.p.1.s.8.w.7', 'WR-P-E-J-0000000001.p.1.s.8.w.8', 'WR-P-E-J-0000000001.p.1.s.8.w.9', 'WR-P-E-J-0000000001.p.1.s.8.w.10', 'WR-P-E-J-0000000001.p.1.s.8.w.11', 'WR-P-E-J-0000000001.p.1.s.8.w.12', 'WR-P-E-J-0000000001.p.1.s.8.w.13', 'WR-P-E-J-0000000001.p.1.s.8.w.14', 'WR-P-E-J-0000000001.p.1.s.8.w.15', 'WR-P-E-J-0000000001.p.1.s.8.w.16', 'WR-P-E-J-0000000001.p.1.s.8.w.17', 'entry.1.term.1.w.1', 'sandbox.list.1.listitem.1.s.1.w.1', 'sandbox.list.1.listitem.1.s.1.w.2', 'sandbox.list.1.listitem.2.s.1.w.1', 'sandbox.list.1.listitem.2.s.1.w.2', 'sandbox.figure.1.caption.s.1.w.1', 'sandbox.figure.1.caption.s.1.w.2', 'WR-P-E-J-0000000001.sandbox.2.s.1.w.1', 'WR-P-E-J-0000000001.sandbox.2.s.1.w.2', 'WR-P-E-J-0000000001.sandbox.2.s.1.w.3', 'WR-P-E-J-0000000001.sandbox.2.s.1.w.4', 'WR-P-E-J-0000000001.sandbox.2.s.1.w.5', 'WR-P-E-J-0000000001.sandbox.2.s.1.w.6', 'WR-P-E-J-0000000001.sandbox.2.s.2.w.1', 'WR-P-E-J-0000000001.sandbox.2.s.2.w.2', 'WR-P-E-J-0000000001.sandbox.2.s.2.w.3', 'WR-P-E-J-0000000001.sandbox.2.s.2.w.4', 'WR-P-E-J-0000000001.sandbox.2.s.2.w.5', 'WR-P-E-J-0000000001.sandbox.2.s.2.w.6', 'WR-P-E-J-0000000001.sandbox.2.s.2.w.7', 'WR-P-E-J-0000000001.sandbox.2.s.2.w.8', 'WR-P-E-J-0000000001.sandbox.2.s.3.w.1', 'WR-P-E-J-0000000001.sandbox.2.s.3.w.2', 'WR-P-E-J-0000000001.sandbox.2.s.3.w.3', 'WR-P-E-J-0000000001.sandbox.2.s.3.w.4', 'WR-P-E-J-0000000001.sandbox.2.s.3.w.6', 'example.table.1.w.1', 'example.table.1.w.2', 'example.table.1.w.3', 'example.table.1.w.4', 'example.table.1.w.5', 'example.table.1.w.6', 'example.table.1.w.7', 'example.table.1.w.8', 'example.table.1.w.9', 'example.table.1.w.10', 'example.table.1.w.11', 'example.table.1.w.12', 'example.table.1.w.13', 'example.table.1.w.14', " );
}

void sanity_test004(){
  startTestSerie(" first word " );
  FoliaElement* w = 0;
  assertNoThrow( w = sanityDoc.words(0) );
  assertTrue( w->isinstance( Word_t ) );
  assertTrue( w->id() == "WR-P-E-J-0000000001.head.1.s.1.w.1" );
  assertTrue( w->text() == "Stemma" );
  assertTrue( str(w) == "Stemma" );
  assertTrue( unicode(w) == "Stemma" );

}

void sanity_test005( ){
  startTestSerie(" last word " );
  FoliaElement* w = 0;
  assertNoThrow( w = sanityDoc.rwords(0) );
  assertTrue( w->isinstance<Word>() );
  assertEqual( w->id(), "example.table.1.w.14" );
  assertEqual( w->text(), "University" );
  assertEqual( str(w), string("University") );
}

void sanity_test006a( ){
  startTestSerie(" second sentence " );
  FoliaElement* s = 0;
  assertNoThrow( s = sanityDoc.sentences(1) );
  assertTrue( isinstance( s, Sentence_t ) );
  assertEqual( s->id(), "WR-P-E-J-0000000001.p.1.s.1" );
  assertTrue( !s->hastext() );
  assertEqual( s->str() , "Stemma is een ander woord voor stamboom." );
}

void sanity_test006b(){
  startTestSerie( " Sanity check - Sentence text (including retaining tokenisation) " );
  // grab fifth sentence
  Sentence *s = sanityDoc.sentences(5);
  assertTrue( isinstance( s, Sentence_t ) );
  assertFalse( s->hastext() );
  assertEqual( s->text(), "De andere handschriften krijgen ook een letter die verband kan houden met hun plaats van oorsprong óf plaats van bewaring." );
  assertEqual( s->text("current",true), "De andere handschriften krijgen ook een letter die verband kan houden met hun plaats van oorsprong óf plaats van bewaring ." );
  // not detokenised
  assertEqual( s->toktext(), "De andere handschriften krijgen ook een letter die verband kan houden met hun plaats van oorsprong óf plaats van bewaring ." );
  // just an alias for the above
}

void sanity_test006c(){
  startTestSerie( " Sanity check - Sentence text both tokenized and not " );
  // grab fourth sentence
  Sentence *s = sanityDoc.sentences(4);
  assertTrue( s->hastext() );
  // The sentence has text of it's own
  assertEqual( s->text(), "De hoofdletter A wordt gebruikt voor het originele handschrift." );
  // The sentence has <w> children with tokenization too
  assertEqual( s->toktext("current"), "De hoofdletter A wordt gebruikt voor het originele handschrift ." );
  assertEqual( s->toktext(), "De hoofdletter A wordt gebruikt voor het originele handschrift ." );
  assertThrow( s->toktext("strange"), NoSuchText );
}

void sanity_test007( ){
  startTestSerie(" use index " );
  FoliaElement *e = 0;
  assertNoThrow( e = sanityDoc["WR-P-E-J-0000000001.p.1.s.2.w.7"] );
  assertTrue( e->isinstance( Word_t ) );
  assertEqual( e, sanityDoc.index("WR-P-E-J-0000000001.p.1.s.2.w.7") );
  assertEqual( e->id(), string("WR-P-E-J-0000000001.p.1.s.2.w.7") );
  assertEqual( e->text(), "stamboom" );

}

void sanity_test008a(){
  startTestSerie(" division + head " );
  FoliaElement *e = 0;
  assertNoThrow( e = sanityDoc["WR-P-E-J-0000000001.div0.1"] );
  assertTrue( e->isinstance( Division_t ) );
  FoliaElement *h = 0;
  assertNoThrow( h = e->head() );
  assertEqual( h,  sanityDoc["WR-P-E-J-0000000001.head.1"] );
  assertTrue( len( h ) == 1 );
}

void sanity_test008b(){
  startTestSerie(" occurrences  " );
  FoliaElement *d = 0;
  assertNoThrow( d = sanityDoc["WR-P-E-J-0000000001.div0.1"] );
  FoliaElement *e = 0;
  assertNoThrow( e = new Head( getArgs(XML_ID"='whatever'"), &sanityDoc ) );
  assertThrow( d->append(e), DuplicateAnnotationError );
}

void sanity_test008c(){
  startTestSerie(" empty text  " );
  string xml = "<?xml version=\"1.0\"?>\n"
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\""
" xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example\" generator=\"libfolia-v0.8\" version=\"0.8\">\n"
"  <metadata type=\"native\">\n"
"    <annotations>\n"
"      <gap-annotation annotator=\"sloot\" set=\"gap-set\"/>\n"
"    </annotations>\n"
"  </metadata>\n"
"  <text xml:id=\"example.text.1\">\n"
"    <p xml:id=\"par\">\n"
"      <w xml:id=\"word\" />\n"
"    </p>"
"  </text>\n"
"</FoLiA>\n" ;

  Document doc;
  assertNoThrow( doc.readFromString(xml) );
  FoliaElement *t = 0;
  // creating an empty text is forbidden
  assertThrow( t = new TextContent( getArgs("value=''"), &doc ), ArgsError );
  // creating a not quite empty text is allowed
  assertNoThrow( t = new TextContent( getArgs("value=' '"), &doc ) );
  FoliaElement *p = 0;
  assertNoThrow( p = doc["par"] );
  // appending a not quite empty text is allowed
  assertNoThrow( p->append(t) );
  FoliaElement *w = 0;
  assertNoThrow( w = doc["word"] );
  // appending a node twice is NOT allowed
  assertThrow( w->append(t), XmlError );
  assertNoThrow( t = new TextContent(  getArgs("value='  '"), &doc ) );
  // appending a not quite empty text is NOT allowed for WORDS!
  assertThrow( w->append(t), ValueError );
}

void sanity_test009( ){
  startTestSerie(" Token Annotation - Pos " );
  FoliaElement *w = 0;
  assertNoThrow( w = sanityDoc.words(0) );
  assertTrue( w->annotation<PosAnnotation>() == w->select<PosAnnotation>()[0] );
  assertTrue( w->annotation<PosAnnotation>()->isinstance(PosAnnotation_t ) );
  assertTrue( isSubClass( PosAnnotation_t, AbstractTokenAnnotation_t ) );
  bool test = isSubClass<PosAnnotation, AbstractTokenAnnotation>();
  assertTrue( test );
  assertTrue( w->annotation<PosAnnotation>()->cls() == "N(soort,ev,basis,onz,stan)" );
  assertTrue( w->pos() == "N(soort,ev,basis,onz,stan)" );
  assertTrue( w->annotation<PosAnnotation>()->sett() == "https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn" );
  assertTrue( w->annotation<PosAnnotation>()->annotator() == "frog" );
  assertTrue( w->annotation<PosAnnotation>()->annotatortype() == AUTO );

}

void sanity_test010( ){
  startTestSerie(" Token Annotation - Lemma " );
  FoliaElement *w = 0;
  assertNoThrow( w = sanityDoc.words(0) );
  FoliaElement *l = 0;
  assertNoThrow( l = w->annotation<LemmaAnnotation>() );
  assertTrue( l == w->select<LemmaAnnotation>()[0] );
  assertTrue( isinstance( l, LemmaAnnotation_t ) );
  assertTrue( l->cls() == "stemma" );
  assertTrue( w->lemma() == "stemma" );
  assertTrue( l->sett() == "lemmas-nl" );
  assertTrue( l->annotator() == "tadpole" );
  assertTrue( l->annotatortype() == AUTO );

}

void sanity_test011( ){
  startTestSerie(" Token Annotation - Non-existing element ");
  FoliaElement *w = 0;
  assertNoThrow( w = sanityDoc.words(0) );
  assertTrue( len( w->select<SenseAnnotation>() ) == 0 );
  assertTrue( w->select<SenseAnnotation>().size() == 0 );
  assertThrow( w->annotation<SenseAnnotation>(),
			NoSuchAnnotation );

}

void sanity_test012( ){
  startTestSerie(" Correction - Text " );
  FoliaElement *w = 0;
  assertNoThrow( w = sanityDoc["WR-P-E-J-0000000001.p.1.s.6.w.31"] );
  FoliaElement *c = 0;
  assertNoThrow( c = w->annotation<Correction>( ) );
  assertTrue( len( c->getNew() ) == 1 );
  assertTrue( len( c->getOriginal() ) == 1 );
  assertTrue( w->text() == "vierkante" );
  assertTrue( c->getNew()->index(0)->text() == "vierkante" );
  assertTrue( c->getNew(0)->text() == "vierkante" );
  assertTrue( c->getOriginal()->index(0)->text() == "vierkant" );
  assertTrue( c->getOriginal(0)->text() == "vierkant" );

}

void sanity_test013( ){
  startTestSerie(" Correction - Token Annotation " );
  FoliaElement *w = 0;
  assertNoThrow( w = sanityDoc["WR-P-E-J-0000000001.p.1.s.6.w.32"] );
  FoliaElement *c = 0;
  assertNoThrow( c = w->annotation<Correction>( ) );
  assertTrue( len( c->getNew() ) == 1 );
  assertTrue( len( c->getOriginal() ) == 1 );
  assertTrue( w->annotation<LemmaAnnotation>()->cls() == "haak" );
  assertTrue( (*c->getNew())[0]->cls() == "haak" );
  assertTrue( (*c->getOriginal())[0]->cls() == "haaak" );
  assertTrue( c->getOriginal(0)->cls() == "haaak" );

}

void sanity_test014(){
  startTestSerie(" Correction - Suggestions (text) " );
  FoliaElement *w = 0;
  assertNoThrow( w = sanityDoc["WR-P-E-J-0000000001.p.1.s.8.w.14"] );
  FoliaElement *c = 0;
  assertNoThrow( c = w->annotation<Correction>( ) );
  assertTrue( c->suggestions().size() == 2 );
  assertTrue( c->suggestions(0)->text() == "twijfelachtige" );
  assertTrue( c->suggestions(1)->text() == "ongewisse" );

}

string repr( FoliaElement *a ){
  if ( a )
    return "[" + toString( a->element_id() ) + "]";
  else
    return "[null]";
}

int check( FoliaElement *parent, const string& indent, ostream& os, int& fails ){
  int count = 0;
  for ( size_t i=0; i < parent->size(); ++i ){
    FoliaElement *child = parent->index(i);
    os << indent << repr( child ) << endl;
    if ( ! ( (parent->isinstance( SyntacticUnit_t )
	      || parent->isinstance( Chunk_t )
	      || parent->isinstance( Entity_t )
	      || parent->isinstance( TimeSegment_t )
	      || parent->isinstance( TimingLayer_t )
	      || parent->isinstance( CoreferenceChain_t )
	      || parent->isinstance( CoreferenceLink_t )
	      || parent->isinstance( SemanticRole_t )
	      || parent->isinstance( Headspan_t )
	      || parent->isinstance( Source_t )
	      || parent->isinstance( Target_t )
	      || parent->isinstance( Relation_t )
	      || parent->isinstance( Sentiment_t )
	      || parent->isinstance( Statement_t )
	      || parent->isinstance( Observation_t )
	      || parent->isinstance( Dependency_t )
	      || parent->isinstance( DependencyDependent_t ) )
	     && ( child->isinstance( Word_t ) || child->isinstance( Morpheme_t )) ) ){
      ++count;
      if ( parent != child->parent() ){
	//	os << indent << "^ DAAR!" << endl;
	++fails;
      }
      count += check( child, indent + " ", os, fails );
    }
  }
  return count;
}

void sanity_test015( ){
  startTestSerie(" Checking if all elements know who's their daddy " );
  ofstream os( "/tmp/foliaparent.txt" );
  int fails = 0;
  int cnt = check( sanityDoc.doc(), "",  os, fails );
  cerr << "         - checked " << cnt << " parents" << endl;
  assertEqual( fails, 0 );
  int stat = system( "diff -b /tmp/foliaparent.txt tests/foliaparent.ok" );
  assertMessage( "/tmp/foliaparent.txt tests/foliaparent.ok differ!",
		 stat == 0 );

}


void sanity_test016a(){
  startTestSerie(" Check - Description " );
  FoliaElement *w = sanityDoc["WR-P-E-J-0000000001.p.1.s.1.w.6"];
  assertTrue( w->description() == "Dit woordje is een voorzetsel, het is maar dat je het weet..." );
}

void sanity_test016b(){
  startTestSerie(" Check - Error on non-existing description " );
  FoliaElement *w = sanityDoc["WR-P-E-J-0000000001.p.1.s.1.w.7"];
  assertEqual( w->description(), "" );

}

void sanity_test017(){
  startTestSerie(" Check - Gap " );
  FoliaElement *gap = sanityDoc["WR-P-E-J-0000000001.gap.1"];
  assertEqual( TiCC::trim(gap->content()).substr(0,11), string("De tekst is") );
  assertTrue( gap->cls() == "backmatter" );
  assertTrue( gap->description() == "Backmatter" );

}

void sanity_test018(){
  startTestSerie(" Subtoken annotation (part of speech) " );
  FoliaElement *w = sanityDoc["WR-P-E-J-0000000001.p.1.s.2.w.5"];
  PosAnnotation *m = 0;
  assertNoThrow( m = w->annotation<PosAnnotation>() );
  assertEqual( m->feat("wvorm"),"pv" );
  assertEqual( m->feat("pvtijd"), "tgw" );
  assertEqual( m->feat("pvagr") , "met-t" );
}

void sanity_test019(){
  startTestSerie(" Alignment in same Document" );
  FoliaElement *w = sanityDoc["WR-P-E-J-0000000001.p.1.s.3.w.10"];
  Alignment *aref = 0;
  assertNoThrow( aref = w->annotation<Alignment>() );
  vector<FoliaElement *>targets;
  assertNoThrow( targets = aref->resolve() );
  assertTrue( targets.size() != 0 );
  assertTrue( targets[0]->xmlstring() == sanityDoc["WR-P-E-J-0000000001.p.1.s.3.w.5"]->xmlstring() );

}

void sanity_test020a(){
  startTestSerie(" Span annotation (Syntax) " );
  FoliaElement *s = sanityDoc["WR-P-E-J-0000000001.p.1.s.1"];
  FoliaElement *l = 0;
  assertNoThrow( l = s->annotation<SyntaxLayer>() );
  assertTrue( isinstance( l->index(0), SyntacticUnit_t ) );
  assertTrue( l->index(0)->cls() == "sentence" );
  assertTrue( l->index(0)->index(0)->cls() == "subject" );
  assertTrue( l->index(0)->index(0)->text() == "Stemma" );
  assertTrue( l->index(0)->index(1)->cls() == "verb" );
  assertTrue( l->index(0)->index(2)->cls() == "predicate" );
  assertTrue( l->index(0)->index(2)->index(0)->cls() == "np" );
  assertTrue( l->index(0)->index(2)->index(1)->cls() == "pp" );
  assertTrue( l->index(0)->index(2)->index(1)->text() == "voor stamboom" );
  assertEqual( l->index(0)->index(2)->text(), "een ander woord voor stamboom" );

}

void sanity_test020b(){
  startTestSerie(" Span annotation (Chunking) " );
  FoliaElement *s = sanityDoc["WR-P-E-J-0000000001.p.1.s.1"];
  ChunkingLayer *l = 0;
  assertNoThrow( l = s->annotation<ChunkingLayer>() );
  assertTrue( isinstance( l->index(0), Chunk_t ) );
  assertTrue( l->index(0)->text() == "een ander woord" );
  assertTrue( l->index(1)->text() == "voor stamboom" );

}

void sanity_test020c(){
  startTestSerie(" Span annotation (Entities) " );
  FoliaElement *s = sanityDoc["WR-P-E-J-0000000001.p.1.s.1"];
  EntitiesLayer *l = 0;
  assertNoThrow( l = s->annotation<EntitiesLayer>() );
  assertTrue( isinstance( l->index(0), Entity_t ) );
  assertTrue( l->index(0)->text() == "ander woord" );

}

void sanity_test020d(){
  startTestSerie(" Span annotation (Dependencies) " );
  FoliaElement *s = sanityDoc["WR-P-E-J-0000000001.p.1.s.1"];
  DependenciesLayer *l = 0;
  assertNoThrow( l = s->annotation<DependenciesLayer>( ) );
  assertTrue( l->size() == 6 );

  assertTrue( l->index(0)->head()->text() == "is" );
  assertTrue( l->index(0)->dependent()->text() == "Stemma" );
  assertTrue( l->index(0)->cls() == "su" );

  assertTrue( l->index(1)->head()->text() == "is" );
  assertTrue( l->index(1)->dependent()->text() == "woord" );
  assertTrue( l->index(1)->cls() == "predc" );

  assertTrue( l->index(2)->head()->text() == "woord" );
  assertTrue( l->index(2)->dependent()->text() == "een" );
  assertTrue( l->index(2)->cls() == "det" );

  assertTrue( l->index(3)->head()->text() == "woord" );
  assertTrue( l->index(3)->dependent()->text() == "ander" );
  assertTrue( l->index(3)->cls() == "mod" );

  assertTrue( l->index(4)->head()->text() == "woord" );
  assertTrue( l->index(4)->dependent()->text() == "voor" );
  assertTrue( l->index(4)->cls() == "mod" );

  assertTrue( l->index(5)->head()->text() == "voor" );
  assertTrue( l->index(5)->dependent()->text() == "stamboom" );
  assertTrue( l->index(5)->cls() == "obj1" );

}

void sanity_test020e(){
  startTestSerie( " Span Annotation (TimeSegment) " );
  FoliaElement *s = sanityDoc["WR-P-E-J-0000000001.p.1.s.1"];
  TimingLayer *l = 0;
  assertNoThrow( l = s->annotation<TimingLayer>() );

  assertTrue( isinstance( l->index(0), TimeSegment_t ) );
  assertEqual( l->index(0)->text(),  "een ander woord" ) ;
  assertEqual( l->index(1)->cls(), "cough" );
  assertEqual( l->index(2)->text(),  "voor stamboom" );
}

void sanity_test020f(){
  startTestSerie( " Span Annotation (Co-reference) " );
  FoliaElement *s = sanityDoc["WR-P-E-J-0000000001.div0.1"];
  DependenciesLayer *l = 0;
  assertNoThrow( l = s->annotation<DependenciesLayer>() );
  vector<Dependency*> deps;
  assertNoThrow( deps = l->annotations<Dependency>() );
  assertEqual( deps[0]->cls(), "su" );
  assertEqual( deps[1]->cls(), "predc" );
  assertEqual( deps[2]->cls(), "det" );
  assertEqual( deps[3]->cls(), "mod" );
  assertEqual( deps[4]->cls(), "mod" );
  assertEqual( deps[5]->cls(), "obj1" );
  assertEqual( deps[2]->head()->wrefs()[0], sanityDoc["WR-P-E-J-0000000001.p.1.s.1.w.5"] );
  assertEqual( deps[2]->dependent()->wrefs()[0], sanityDoc["WR-P-E-J-0000000001.p.1.s.1.w.3"] );
}

void sanity_test020g(){
  startTestSerie( " Semantic Role Labelling " );
  FoliaElement *s = sanityDoc["WR-P-E-J-0000000001.p.1.s.7"];
  SemanticRolesLayer *l = 0;
  assertNoThrow( l = s->annotation<SemanticRolesLayer>() );
  vector<SemanticRole*> roles = l->annotations<SemanticRole>();
  assertEqual( roles[0]->cls(), "actor" );
  assertEqual( roles[1]->cls(),  "patient" );
  assertEqual( roles[0]->wrefs(0), sanityDoc["WR-P-E-J-0000000001.p.1.s.7.w.3"] );
  assertEqual( roles[1]->wrefs(0), sanityDoc["WR-P-E-J-0000000001.p.1.s.7.w.4"] );
  assertEqual( roles[1]->wrefs(1), sanityDoc["WR-P-E-J-0000000001.p.1.s.7.w.5"] );
}

void sanity_test021(){
  startTestSerie(" Obtaining previous word " );
  FoliaElement *w = sanityDoc["WR-P-E-J-0000000001.p.1.s.2.w.7"];
  FoliaElement *prevw = w->previous();
  assertTrue( prevw->isinstance( Word_t ) );
  assertTrue( prevw->text() == "zo'n" );

}

void sanity_test022(){
  startTestSerie(" Obtaining next word " );
  FoliaElement *w = sanityDoc["WR-P-E-J-0000000001.p.1.s.2.w.7"];
  FoliaElement *prevw = w->next();
  assertTrue( prevw->isinstance( Word_t ) );
  assertTrue( prevw->text() == "," );

}

void sanity_test023a(){
  startTestSerie(" Obtaining left context " );
  FoliaElement *w = sanityDoc["WR-P-E-J-0000000001.p.1.s.2.w.7"];
  vector<Word*> context = w->leftcontext(3);
  assertTrue( context.size() == 3 );
  assertTrue( context[0]->text() == "wetenschap" );
  assertTrue( context[1]->text() == "wordt" );
  assertTrue( context[2]->text() == "zo'n" );

}

void sanity_test023b(){
  startTestSerie(" Obtaining left context with default placeholder " );
  FoliaElement *w = sanityDoc["WR-P-E-J-0000000001.p.1.s.1.w.2"];
  vector<Word*> context = w->leftcontext(3);
  assertTrue( context.size() == 3 );
  assertTrue( context[0] == 0 );                // no words
  assertTrue( context[1]->text() == "Stemma" ); // from the head
  assertTrue( context[2]->text() == "Stemma" ); // w.1

}

void sanity_test023c(){
  startTestSerie(" Obtaining left context with placeholder " );
  FoliaElement *w = sanityDoc["WR-P-E-J-0000000001.p.1.s.1.w.2"];
  vector<Word*> context = w->leftcontext(3,"?");
  assertTrue( context.size() == 3 );
  assertTrue( context[0]->isinstance( PlaceHolder_t) );
  assertTrue( context[0]->text() == "?" );
  assertTrue( context[1]->text() == "Stemma" );
  assertTrue( context[2]->text() == "Stemma" );

}

void sanity_test024a(){
  startTestSerie(" Obtaining right context " );
  FoliaElement *w = sanityDoc["WR-P-E-J-0000000001.p.1.s.2.w.7"];
  vector<Word*> context = w->rightcontext(3);
  assertTrue( context.size() == 3 );
  assertTrue( text(context[0]) == "," );
  assertTrue( text(context[1]) == "onder" );
  assertTrue( text(context[2]) == "de" );

}

void sanity_test024b(){
  startTestSerie(" Obtaining right context with default placeholder " );
  FoliaElement *w = sanityDoc["example.table.1.w.13"];
  vector<Word*> context = w->rightcontext(3);
  assertTrue( context.size() == 3 );
  assertTrue( text(context[0]) == "University" );
  assertTrue( context[1] == 0 );
  assertTrue( context[2] == 0 );
}

void sanity_test024c(){
  startTestSerie(" Obtaining right context with placeholder " );
  FoliaElement *w = sanityDoc["example.table.1.w.13"];
  vector<Word*> context = w->rightcontext(3, "_");
  assertTrue( context.size() == 3 );
  assertTrue( text(context[0]) == "University" );
  assertTrue( context[1]->isinstance(PlaceHolder_t) );
  assertTrue( context[2]->text() == "_" );

}

void sanity_test025a(){
  startTestSerie(" Obtaining full context " );
  FoliaElement *w = sanityDoc["WR-P-E-J-0000000001.p.1.s.2.w.7"];
  vector<Word*> context = w->context(3);
  assertTrue( context.size() == 7 );
  assertTrue( context[0]->text() == "wetenschap" );
  assertTrue( context[1]->text() == "wordt" );
  assertTrue( context[2]->text() == "zo'n" );
  assertTrue( text(context[3]) == "stamboom" );
  assertTrue( text(context[4]) == "," );
  assertTrue( text(context[5]) == "onder" );
  assertTrue( text(context[6]) == "de" );

}

void sanity_test025b(){
  startTestSerie(" Obtaining full context with default placeholder " );
  FoliaElement *w = sanityDoc["WR-P-E-J-0000000001.p.1.s.2.w.7"];
  vector<Word*> context = w->context(250);
  assertTrue( context.size() == 501 );
  assertTrue( context[0] == 0 );
  assertEqual( text(context[301]), "handschrift" );
  assertTrue( context[500] == 0 );

}

void sanity_test025c(){
  startTestSerie(" Obtaining full context with string placeholder " );
  FoliaElement *w = sanityDoc["WR-P-E-J-0000000001.p.1.s.2.w.7"];
  vector<Word*> context = w->context(250, "nil");
  assertTrue( context.size() == 501 );
  assertTrue( text(context[0]) == "nil" );
  assertEqual( text(context[301]), "handschrift" );
  assertTrue( text(context[202]) == "nil" );
  assertTrue( context[500]->isinstance(PlaceHolder_t) );

}

void sanity_test026a(){
  startTestSerie(" Features " );
  FoliaElement *w = sanityDoc["WR-P-E-J-0000000001.p.1.s.6.w.1"];
  FoliaElement *pos = w->annotation<PosAnnotation>();
  assertTrue( pos->isSubClass( AbstractTokenAnnotation_t ) );
  assertTrue( pos->isinstance(PosAnnotation_t) );
  assertTrue( pos->cls() == "WW(vd,prenom,zonder)" );
  assertTrue( len(pos) ==  1 );
  vector<Feature*> features = pos->select<Feature>();
  assertTrue( len(features) == 1 );
  assertFalse( isSubClass( features[0]->element_id(), Sentence_t ) );
  assertTrue( isSubClass( features[0]->element_id(), Feature_t ) );
  assertTrue( isinstance(features[0], Feature_t ) );
  assertTrue( features[0]->subset() == "head" );
  assertTrue( features[0]->cls() == "WW" );
  KWargs args;
  args["subset"] = "subset";
  args["class"] = "";
  assertThrow( new Feature( args ), ValueError );
  args["subset"] = "";
  args["class"] = "class";
  assertThrow( new Feature( args ), ValueError );
}

void sanity_test026b(){
  startTestSerie(" Metric " );
  FoliaElement *p= sanityDoc.paragraphs(0);
  FoliaElement *m = p->annotation<Metric>();
  assertTrue( isinstance(m, Metric_t ) );
  assertEqual( m->cls(), "sentenceCount" );
  assertEqual( m->feat("value"), "8" );
}

void sanity_test027(){
  startTestSerie(" Time Stamp " );
  FoliaElement *word = sanityDoc["WR-P-E-J-0000000001.p.1.s.8.w.15"];
  FoliaElement *pos = word->annotation<PosAnnotation>();
  assertTrue( pos->getDateTime() == "2011-07-20T19:00:01" );
  assertTrue( pos->xmlstring() == "<pos xmlns=\"http://ilk.uvt.nl/folia\" class=\"N(soort,ev,basis,zijd,stan)\" datetime=\"2011-07-20T19:00:01\" set=\"https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn\"/>" );

}

void sanity_test028() {
  startTestSerie(" Finding parents of word " );
  FoliaElement *w = sanityDoc["WR-P-E-J-0000000001.p.1.s.8.w.15"];

  FoliaElement *s = w->sentence();
  assertTrue( s->isinstance( Sentence_t ) );
  assertTrue( s->id() == "WR-P-E-J-0000000001.p.1.s.8" );

  FoliaElement *p = w->paragraph();
  assertTrue( isinstance( p, Paragraph_t ) );
  assertTrue( p->id() == "WR-P-E-J-0000000001.p.1" );

  FoliaElement *div = w->division();
  assertTrue( isinstance( div, Division_t ) );
  assertTrue( div->id() == "WR-P-E-J-0000000001.div0.1" );
  assertTrue( w->incorrection() == 0 );

}

void sanity_test029(){
  startTestSerie(" Sanity Check - Quote " );
  FoliaElement *q = sanityDoc["WR-P-E-J-0000000001.p.1.s.8.q.1"];
  assertTrue( q->isinstance( Quote_t ) );
  assertTrue( q->text() == "volle lijn" );

  FoliaElement *s = sanityDoc["WR-P-E-J-0000000001.p.1.s.8"];
  assertTrue( s->text() == "Een volle lijn duidt op een verwantschap , terweil een stippelijn op een onzekere verwantschap duidt ." );
  // (spelling errors are present in sentence)

  // get a word from the quote
  FoliaElement *w = sanityDoc["WR-P-E-J-0000000001.p.1.s.8.w.2"];
  // and check if sentence matches
  assertTrue( w->sentence() == s );

}

void sanity_test030( ){
  startTestSerie(" Text Content " );
  FoliaElement *s = sanityDoc["WR-P-E-J-0000000001.p.1.s.4"];
  assertEqual( s->text(), "De hoofdletter A wordt gebruikt voor het originele handschrift." );
  assertEqual( s->stricttext(), "De hoofdletter A wordt gebruikt voor het originele handschrift." );
  assertEqual( s->stricttext("original"), "De hoofdletter A wordt gebruikt voor het originele handschrift." );
  assertThrow( s->text( "BLAH" ), NoSuchText );

  FoliaElement *w = sanityDoc["WR-P-E-J-0000000001.p.1.s.4.w.2"];
  assertTrue( w->text() == "hoofdletter" );
  const TextContent* t = 0;
  assertNoThrow( t = w->textcontent() );
  assertNoThrow( t = t->textcontent() ); // should deliver itself!
  assertTrue( t->text() == "hoofdletter" );
  assertTrue( t->offset() == 3 );

  FoliaElement *w2= sanityDoc["WR-P-E-J-0000000001.p.1.s.6.w.31"];
  assertTrue( w2->text() == "vierkante" );
  assertTrue( w2->stricttext() == "vierkante" );

}

void sanity_test030b( ){
  startTestSerie( " Sanity check - Text Content (2) " );
  FoliaElement *head = sanityDoc["sandbox.3.head"];
  const TextContent *t = head->textcontent();
  assertTrue( len(t) == 3 );
  assertEqual( t->text(), "De \nFoLiA developers zijn:" );
  assertEqual( t->index(0)->text(), "De ");
  assertTrue( isinstance( t->index(1), TextMarkupString_t ) );
  assertEqual( t->index(1)->text(), "\nFoLiA developers" );
  assertEqual( t->index(2)->text(), " zijn:" );
}

void sanity_test031( ){
  startTestSerie(" Lexical Semantic Sense Annotation " );
  FoliaElement *w = sanityDoc["sandbox.list.1.listitem.1.s.1.w.1"];
  FoliaElement *sense = w->annotation<SenseAnnotation>( );
  assertTrue( sense->cls() == "some.sense.id" );
  assertTrue( sense->feat("synset") == "some.synset.id" );

}

void sanity_test032( ){
  startTestSerie(" Events " );
  FoliaElement *l = sanityDoc["sandbox"];
  FoliaElement *event = l->annotation<Event>();
  assertTrue( event->cls() == "applause" );
  assertTrue( event->feat("actor") == "audience" );

}

void sanity_test033( ){
  startTestSerie(" List " );
  FoliaElement *l = sanityDoc["sandbox.list.1"];
  assertTrue( l->index(0)->isinstance<ListItem>() );
  assertTrue( l->index(0)->n() == "1" );
  assertTrue( l->index(0)->text() == "Eerste testitem" );
  assertTrue( l->rindex(0)->isinstance<ListItem>() );
  assertTrue( l->rindex(1)->n() == "2" );
  assertTrue( l->rindex(1)->text() == "Tweede testitem" );

}

void sanity_test034( ){
  startTestSerie(" Figure " );
  FoliaElement *fig = sanityDoc["sandbox.figure.1"];
  assertTrue( fig->src() == "http://upload.wikimedia.org/wikipedia/commons/8/8e/Family_tree.svg" );
  assertTrue( fig->caption() == "Een stamboom" );

}

void sanity_test035( ){
  startTestSerie(" Event " );
  FoliaElement *e = sanityDoc["sandbox.event.1"];
  assertTrue( e->feat("actor") == "proycon" );
  assertTrue( e->feat("begindatetime") == "2011-12-15T19:01" );
  assertTrue( e->feat("enddatetime") == "2011-12-15T19:05" );

}

void sanity_test036( ){
  startTestSerie(" Paragraph and Sentence annotation " );
  FoliaElement *e = sanityDoc["WR-P-E-J-0000000001.p.1"];
  assertTrue( e->cls() == "firstparagraph" );
  e = sanityDoc["WR-P-E-J-0000000001.p.1.s.6"];
  assertTrue( e->cls() == "sentence" );

}

void sanity_test037a( ){
  startTestSerie(" Feature test & Ambiguitity resolution of head as PoS Feature and as structure element " );
  string xml = "<?xml version=\"1.0\"?>\n"
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\""
" xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example\" generator=\"libfolia-v0.8\" version=\"0.8\">\n"
"  <metadata src=\"test.cmdi.xml\" type=\"cmdi\">\n"
"    <annotations>\n"
"      <pos-annotation set=\"test\"/>\n"
"    </annotations>\n"
"  </metadata>\n"
"  <text xml:id=\"test.text\">\n"
"    <div xml:id=\"div\">\n"
"     <head xml:id=\"head\">\n"
"       <s xml:id=\"head.1.s.1\">\n"
"            <w xml:id=\"head.1.s.1.w.1\">\n"
"                <t>blah</t>\n"
"                <pos class=\"NN(blah)\" head=\"NN\" />\n"
"            </w>\n"
"        </s>\n"
"    </head>\n"
"    <p xml:id=\"p.1\">\n"
"        <s xml:id=\"p.1.s.1\">\n"
"            <w xml:id=\"p.1.s.1.w.1\">\n"
"                <t>blah</t>\n"
"                <pos class=\"BB(blah)\">\n"
"                    <feat subset=\"head\" class=\"BB\" />\n"
"                </pos>\n"
"            </w>\n"
"        </s>\n"
"    </p>\n"
"   </div>\n"
"  </text>\n"
"</FoLiA>\n" ;

  Document doc;
  assertNoThrow( doc.readFromString(xml) );
  assertTrue( doc["head.1.s.1.w.1"]->pos() == "NN(blah)" );
  assertTrue( doc["head.1.s.1.w.1"]->annotation<PosAnnotation>()->feat("head") == "NN" );
  assertTrue( doc["p.1.s.1.w.1"]->pos() == "BB(blah)" );
  assertTrue( doc["p.1.s.1.w.1"]->annotation<PosAnnotation>()->feat("head") == "BB" );
}

void sanity_test037b( ){
  startTestSerie(" Multiclass Feature " );
  string xml = "<?xml version=\"1.0\"?>\n"
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\" "
"xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example\" generator=\"libfolia-v0.8\" version=\"0.8\">\n"
"  <metadata src=\"test.cmdi.xml\" type=\"cmdi\">\n"
"    <annotations>\n"
"      <pos-annotation set=\"test\"/>\n"
"    </annotations>\n"
"  </metadata>\n"
"  <text xml:id=\"test.text\">\n"
"    <div xml:id=\"div\">\n"
"     <p xml:id=\"p.1\">\n"
"       <s xml:id=\"p.1.s.1\">\n"
"         <w xml:id=\"p.1.s.1.w.1\">\n"
"           <t>blah</t>\n"
"           <pos class=\"NN(a,b,c)\">\n"
"           <feat subset=\"x\" class=\"a\" />\n"
"           <feat subset=\"x\" class=\"b\" />\n"
"           <feat subset=\"x\" class=\"c\" />\n"
"           </pos>\n"
"         </w>\n"
"       </s>\n"
"     </p>\n"
"   </div>\n"
"  </text>\n"
"</FoLiA>\n" ;

  Document doc;
  assertNoThrow( doc.readFromString(xml) );
  assertTrue( doc["p.1.s.1.w.1"]->pos() == "NN(a,b,c)" );
  PosAnnotation* p = doc["p.1.s.1.w.1"]->annotation<PosAnnotation>();
  vector<string> v = p->feats("x");
  vector<string> vok = {"a","b","c"};
  assertTrue( v == vok );
}

void sanity_test037c( ){
  startTestSerie(" Multiclass Feature for a known Feature" );
  string xml = "<?xml version=\"1.0\"?>\n"
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\" "
"xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example\" generator=\"libfolia-v0.8\" version=\"0.8\">\n"
"  <metadata src=\"test.cmdi.xml\" type=\"cmdi\">\n"
"    <annotations>\n"
"      <pos-annotation set=\"test\"/>\n"
"    </annotations>\n"
"  </metadata>\n"
"  <text xml:id=\"test.text\">\n"
"    <div xml:id=\"div\">\n"
"     <p xml:id=\"p.1\">\n"
"       <s xml:id=\"p.1.s.1\">\n"
"         <w xml:id=\"p.1.s.1.w.1\">\n"
"           <t>blah</t>\n"
"           <pos class=\"NN(a,b,c)\">\n"
"           <feat subset=\"author\" class=\"Jan\" />\n"
"           <feat subset=\"author\" class=\"Piet\" />\n"
"           <feat subset=\"author\" class=\"Klaas\" />\n"
"           </pos>\n"
"         </w>\n"
"       </s>\n"
"     </p>\n"
"   </div>\n"
"  </text>\n"
"</FoLiA>\n" ;

  Document doc;
  assertNoThrow( doc.readFromString(xml) );
  assertTrue( doc["p.1.s.1.w.1"]->pos() == "NN(a,b,c)" );
  PosAnnotation* pos = doc["p.1.s.1.w.1"]->annotation<PosAnnotation>();
  vector<string> v = pos->feats("author");
  vector<string> vok = {"Jan","Piet","Klaas"};
  assertTrue( v == vok );
  string xml_out = pos->xmlstring();
  assertEqual( xml_out, "<pos xmlns=\"http://ilk.uvt.nl/folia\" class=\"NN(a,b,c)\"><feat class=\"Jan\" subset=\"author\"/><feat class=\"Piet\" subset=\"author\"/><feat class=\"Klaas\" subset=\"author\"/></pos>" );
}

void sanity_test038a(){
  startTestSerie( "Sanity check - Obtaining annotation should not descend into morphology layer" );
  PosAnnotation *p =0;
  assertThrow( p = sanityDoc["WR-P-E-J-0000000001.sandbox.2.s.1.w.2"]->annotation<PosAnnotation>(), NoSuchAnnotation );
}

void sanity_test038b(){
  startTestSerie( "Sanity check - Obtaining morphemes and token annotation under morphemes" );
  FoliaElement *w = sanityDoc["WR-P-E-J-0000000001.sandbox.2.s.1.w.2"];
  vector<Morpheme*> l = w->morphemes(); //get all morphemes
  assertEqual( l.size(), 2 );
  Morpheme *m = w->morpheme(1); // #get second morpheme
  assertEqual( m->annotation<PosAnnotation>()->cls(), "n" );
}

void sanity_test039(){
  startTestSerie( "Sanity Check - Find span on layer" );
  FoliaElement *s = sanityDoc["WR-P-E-J-0000000001.p.1.s.7"];
  SemanticRolesLayer *semrolelayer = s->annotation<SemanticRolesLayer>();
  auto roles = semrolelayer->annotations<SemanticRole>();
  vector<FoliaElement*> wv;
  wv.push_back( sanityDoc["WR-P-E-J-0000000001.p.1.s.7.w.4"] );
  wv.push_back( sanityDoc["WR-P-E-J-0000000001.p.1.s.7.w.5"] );
  assertEqual( semrolelayer->findspan( wv ), roles[1] );
}

void sanity_test040(){
  startTestSerie( "Sanity Check - Iteration over spans" );
  FoliaElement *sentence = sanityDoc["WR-P-E-J-0000000001.p.1.s.1"];
  UnicodeString res;
  for ( const auto& lay : sentence->select<EntitiesLayer>() ){
    for ( const auto& ent : lay->select<Entity>() ){
      for ( const auto& word : ent->wrefs() ){
	res += " " + word->text();
      }
    }
  }
  assertEqual( res, " ander woord" );
}

void sanity_test041a(){
  startTestSerie( "Sanity check - Find spans given words (no set)" );
  UnicodeString res;
  FoliaElement *word = sanityDoc["WR-P-E-J-0000000001.p.1.s.1.w.4"];
  vector<AbstractSpanAnnotation*> spans;
  assertNoThrow( spans = word->findspans<EntitiesLayer>() );
  for( const auto& span: spans ){
    for ( const auto& word: span->wrefs() ){
      res += " " + word->text();
    }
  }
  assertEqual( res, " ander woord" );
}

void sanity_test041b(){
  startTestSerie( "Sanity check - Find spans given words (specific set)" );
  UnicodeString res;
  FoliaElement *word = sanityDoc["example.table.1.w.3"];
  vector<AbstractSpanAnnotation*> spans;
  assertNoThrow( spans = word->findspans<EntitiesLayer>("http://raw.github.com/proycon/folia/master/setdefinitions/namedentities.foliaset.xml") );
  for( const auto& span: spans ){
    for ( const auto& word: span->wrefs() ){
      res += " " + word->text();
    }
  }
  assertEqual( res, " Maarten van Gompel" );
}

void sanity_test041c(){
  startTestSerie( "Sanity check - Find spans given words (specific set, Direct Entities)" );
  UnicodeString res;
  FoliaElement *word = sanityDoc["example.table.1.w.3"];
  vector<AbstractSpanAnnotation*> spans;
  assertNoThrow( spans = word->findspans<Entity>("http://raw.github.com/proycon/folia/master/setdefinitions/namedentities.foliaset.xml") );
  for( const auto& span: spans ){
    for ( const auto& word: span->wrefs() ){
      res += " " + word->text();
    }
  }
  assertEqual( res, " Maarten van Gompel" );
}

void sanity_test042(){
  startTestSerie( " Sanity check - Table " );
  FoliaElement *table = sanityDoc["example.table.1"];
  assertTrue( isinstance( table, Table_t) );
  assertTrue( isinstance( table->index(0), TableHead_t) );
  assertTrue( isinstance( table->index(0)->index(0), Row_t) );
  assertEqual( len( table->index(0)->index(0)), size_t(2) ); // two cells
  assertTrue( isinstance(table->index(0)->index(0)->index(0), Cell_t) );
  assertEqual( table->index(0)->index(0)->index(0)->text(), "Naam" );
  assertEqual( table->index(0)->index(0)->text(), "Naam | Universiteit" ); //text of whole row
}

void sanity_test043(){
  startTestSerie( " Sanity check - String " );
  FoliaElement *head = sanityDoc["sandbox.3.head"];
  assertTrue( head->hasannotation<String>() > 0 );
  vector<String*> v = head->select<String>();
  String* st = v[0];
  assertEqual( st->text(), "FoLiA developers" );
  assertEqual( st->annotation<LangAnnotation>()->cls(), "eng" );
  assertEqual( st->language(), "eng" );
  FoliaElement *wrd = sanityDoc["WR-P-E-J-0000000001.p.1.s.7.w.8"];
  assertEqual( wrd->language(), "nl" );
}

void sanity_test044a(){
  startTestSerie( " Sanity check - Text Markup " );
  FoliaElement *head = sanityDoc["sandbox.3.head"];
  const TextContent *t = head->textcontent();
  assertEqual( len(head->select<TextMarkupString>()), size_t(1) );
  assertEqual( len(t->select<TextMarkupString>()), size_t(1) );
  vector<TextMarkupString*> v = t->select<TextMarkupString>();
  TextMarkupString *st = v[0];
  // testing value (full text value)
  assertEqual( st->text(), "\nFoLiA developers" ) ;
  const FoliaElement *r1 =  st->resolveid();
  FoliaElement *r2 = sanityDoc["sandbox.3.str"];
  assertEqual( r1, r2 ); // testing resolving references
  t = sanityDoc["WR-P-E-J-0000000001.p.1.s.6"]->textcontent();
  assertTrue( t->index(t->size()-1)->isinstance( Linebreak_t) );  // did we get the linebreak properly?
  // testing nesting
  assertEqual( len(st), size_t(2) );
  assertEqual( st->index(0), sanityDoc["sandbox.3.str.bold"] );
  // String should support str()
  assertEqual( r2->str(), "FoLiA developers" );
  // testing TextMarkup.text()
  assertEqual( st->index(0)->text(), "\nFoLiA" );

  // resolving returns self if it's not a reference
  assertEqual( sanityDoc["sandbox.3.str.bold"]->resolveid(),
	       sanityDoc["sandbox.3.str.bold"] );
}

void sanity_test044b(){
  startTestSerie( " Sanity check - Text Markup (LineBreak)" );
  FoliaElement *par = sanityDoc["entry.1.def.1.p.1"];
  string txt = par->str();
  assertEqual( txt, "Term uit de editiewetenschap en tekstkritiek voor de schematische weergave in de vorm van een boomdiagram van de onderlinge verwantschap van \n documentaire bronnen (handschrift, codex, druk), waarin een tekst bewaard is gebleven.\n" );
}

void sanity_test045(){
  startTestSerie( " Sanity check - Corrections over Span elements" );
  FoliaElement *cell = sanityDoc["example.last.cell"];
  vector<Entity*> v = cell->select<Entity>();
  assertEqual( len(v), size_t(1) );
  assertEqual( v[0]->id(), "example.tilburg.university.org" );
}

void sanity_test046(){
  startTestSerie( "Sanity Check - Checking entry, term, definition and example");
  FoliaElement *entry = sanityDoc["entry.1"];
  vector<Term*> terms = entry->select<Term>();
  assertEqual( len(terms), 1 );
  assertEqual( terms[0]->text() ,"Stemma" );
  vector<Definition*> definitions = entry->select<Definition>();
  assertEqual( len(definitions), 2 );
  vector<Example*> examples = entry->select<Example>();
  assertEqual( len(examples), 1 );
}

void sanity_test046a(){
  startTestSerie( "Sanity Check - Text serialisation test with linebreaks and whitespaces" );
  FoliaElement *p = sanityDoc["WR-P-E-J-0000000001.p.1"];
    // this is a bit of a malformed paragraph due to the explicit whitespace
    // and linebreaks in it, but makes for a nice test
  assertEqual( p->text(), "Stemma is een ander woord voor stamboom. In de historische wetenschap wordt zo'n stamboom , onder de naam stemma codicum ( handschriftelijke genealogie ) , gebruikt om de verwantschap tussen handschriften weer te geven . \n\nWerkwijze\n\nHiervoor worden de handschriften genummerd en gedateerd zodat ze op de juiste plaats van hun afstammingsgeschiedenis geplaatst kunnen worden . De hoofdletter A wordt gebruikt voor het originele handschrift. De andere handschriften krijgen ook een letter die verband kan houden met hun plaats van oorsprong óf plaats van bewaring. Verdwenen handschriften waarvan men toch vermoedt dat ze ooit bestaan hebben worden ook in het stemma opgenomen en worden weergegeven door de laatste letters van het alfabet en worden tussen vierkante haken geplaatst.\nTenslotte gaat men de verwantschap tussen de handschriften aanduiden . Een volle lijn duidt op een verwantschap , terweil een stippelijn op een onzekere verwantschap duidt .");
}


void sanity_test046b(){
  startTestSerie( "Sanity Check - Text serialisation on lists" );
  FoliaElement *l = sanityDoc["sandbox.list.1"];
  // this is a bit of a malformed paragraph due to the explicit whitespace
  // and linebreaks in it, but makes for a nice test.
  assertEqual( l->text(), "Eerste testitem\nTweede testitem");
}

void sanity_test047(){
  startTestSerie( "Sanity check - Alignment" );
  FoliaElement *word = sanityDoc["WR-P-E-J-0000000001.p.1.s.3.w.10"];
  FoliaElement *a = word->annotation<Alignment>();
  assertEqual( a->cls(), "reference" );
  vector<AlignReference*> refs = a->select<AlignReference>();
  AlignReference *aref = refs[0];
  assertEqual( aref->refid(), "WR-P-E-J-0000000001.p.1.s.3.w.5" );
  assertEqual( aref->type(), "w" );
  assertEqual( aref->t(), "handschriften" );
}

void sanity_test048(){
  startTestSerie( "Sanity check - Observations" );
  FoliaElement *word = sanityDoc["WR-P-E-J-0000000001.p.1.s.8.w.9"];
  vector<AbstractSpanAnnotation*> observations;
  assertNoThrow( observations = word->findspans<ObservationLayer>() );
  assertEqual( observations[0]->cls() , "ei_ij_error" );
  assertEqual( observations[0]->description() , "Confusion between EI and IJ diphtongues");
}

void sanity_test049(){
  startTestSerie( "Sanity check - Sentiments" );
  FoliaElement *sentence = sanityDoc["WR-P-E-J-0000000001.sandbox.2.s.3"];
  SentimentLayer *sentiments = sentence->annotation<SentimentLayer>();
  Sentiment *sentiment = sentiments->annotation<Sentiment>();
  assertEqual( sentiment->cls() , "disappointment" );
  assertEqual( sentiment->feat("polarity") , "negative");
  assertEqual( sentiment->feat("strength") , "strong");
  assertEqual( sentiment->annotation<Source>()->text(), "Hij");
  assertEqual( sentiment->annotation<Headspan>()->text(), "erg teleurgesteld");
}

void sanity_test050(){
  startTestSerie( "Sanity check - Statements" );
  FoliaElement *sentence = sanityDoc["WR-P-E-J-0000000001.sandbox.2.s.2"];
  StatementLayer *statements = sentence->annotation<StatementLayer>();
  Statement *statement = statements->annotation<Statement>();
  assertEqual( statement->cls() , "promise" );
  assertEqual( statement->annotation<Source>()->text(), "Hij");
  assertEqual( statement->annotation<Headspan>()->text(), "hij zou winnen");
}

void sanity_test099(){
  startTestSerie(" Writing to file " );
  assertNoThrow( sanityDoc.save( "/tmp/savetest.xml" ) );
  assertNoThrow( sanityDoc.save( "/tmp/savetest.canonical.xml", true ) );
}

void sanity_test100a( ){
  startTestSerie(" Checking saved file against document " );
  {
    Document d;
    assertNoThrow( d.readFromFile( "/tmp/savetest.xml" ) );
    assertTrue( d == sanityDoc );
  }
  {
    Document d;
    assertNoThrow( d.readFromFile( "/tmp/savetest.canonical.xml" ) );
    assertTrue( d == sanityDoc );
  }
}

void sanity_test100b( ){
  startTestSerie(" Checking saved file against input file " );
  int stat = system( "./tests/foliadiff.sh /tmp/savetest.xml tests/example.xml" );
  assertMessage( "/tmp/savetest.xml tests/example.xml differ!",
		 stat == 0 );

}

void sanity_test101( ){
  startTestSerie(" Add a sentence at wrong position " );
  FoliaElement *p = sanityDoc["WR-P-E-J-0000000001.p.1.s.2.w.7"];
  FoliaElement *s = 0;
  assertNoThrow( s = new Sentence( getArgs( "generate_id='" + p->id() + "'" ),
				    &sanityDoc ) );
  assertThrow( p->append( s ), ValueError );

}

void sanity_test101a(){
  startTestSerie(" Metadata external reference (CMDI) " );
  Document doc( "file='tests/folia.cmdi.xml'" );
  assertEqual( doc.metadatatype(), "cmdi" );
  assertEqual( doc.metadatafile(), "test.cmdi.xml" );
  assertNoThrow( doc.save( "/tmp/folia.cmdi.xml" ) );
  int stat = system( "./tests/foliadiff.sh /tmp/folia.cmdi.xml tests/folia.cmdi.xml" );
  assertMessage( "/tmp/folia.cmdi.xml tests/folia.cmdi.xml differ!",
		 stat == 0 );
  assertThrow( doc.set_metadata( "language", "por" ), MetaDataError );
}

void sanity_test101b(){
  startTestSerie(" Metadata external reference (IMDI) " );
  Document doc( "file='tests/folia.imdi.xml'" );
  assertEqual( doc.metadatatype(), "imdi" );
  assertEqual( doc.metadatafile(), "" );
  assertNoThrow( doc.save( "/tmp/folia.imdi.xml" ) );
  int stat = system( "./tests/foliadiff.sh /tmp/folia.imdi.xml tests/folia.imdi.xml" );
  assertMessage( "/tmp/folia.imdi.xml tests/folia.imdi.xml differ!",
		 stat == 0 );
  assertThrow( doc.set_metadata( "language", "por" ), MetaDataError );
}

void sanity_test101c(){
  startTestSerie(" Metadata (native) " );
  Document doc( "file='tests/example.xml'" );
  assertTrue( doc.metadatatype() == "native" );
  assertNoThrow( doc.set_metadata( "name", "Mijn document" ) );
  assertEqual( doc.get_metadata( "genre" ), "artikel" );
}

void sanity_test101d(){
  startTestSerie(" Metadata (foreign) " );
  Document doc( "file='tests/folia.foreign.xml'" );
  assertTrue( doc.metadatatype() == "pm" );
  assertNoThrow( doc.save( "/tmp/saveforeign.xml" ) );
  int stat = system( "./tests/foliadiff.sh /tmp/saveforeign.xml tests/folia.foreign.xml" );
  assertMessage( "/tmp/saveforeign.xml tests/folia.foreign.xml differ!",
		 stat == 0 );
}

void sanity_test101e(){
  startTestSerie(" Metadata (foreign with namespace) " );
  Document doc( "file='tests/folia.foreign2.xml'" );
  assertTrue( doc.metadatatype() == "pm" );
  assertNoThrow( doc.save( "/tmp/saveforeign2.xml" ) );
  int stat = system( "./tests/foliadiff.sh /tmp/saveforeign2.xml tests/folia.foreign2.xml" );
  assertMessage( "/tmp/saveforeign2.xml tests/folia.foreign2.xml differ!",
		 stat == 0 );
}

void sanity_test101f(){
  startTestSerie(" Metadata (set from external XML) " );
  Document f_doc( "file='tests/minimal.xml'" );
  xmlDoc *x_doc = xmlReadFile( "tests/foreignmeta.xml", NULL, 0 );
  xmlNode *root = xmlDocGetRootElement( x_doc );
  assertNoThrow( f_doc.set_foreign_metadata( root ) );
  assertNoThrow( f_doc.save( "/tmp/foreignmeta.out" ) );
  int stat = system( "./tests/foliadiff.sh /tmp/foreignmeta.out tests/foreignmeta.out" );
  assertMessage( "/tmp/foreignmeta.out tests/foreignmeta.out differ!",
		 stat == 0 );
  assertNoThrow( f_doc.set_foreign_metadata( root ) ); // append the same again
  assertNoThrow( f_doc.save( "/tmp/foreignmeta3.out" ) );
  int stat2 = system( "./tests/foliadiff.sh /tmp/foreignmeta3.out tests/foreignmeta3.out" );
  assertMessage( "/tmp/foreignmeta3.out tests/foreignmeta3.out differ!",
		 stat2 == 0 );
  xmlFreeDoc( x_doc );
}

void sanity_test101g(){
  startTestSerie(" Metadata (set from external foreign-data) " );
  Document f_doc( "file='tests/minimal.xml'" );
  xmlDoc *x_doc = xmlReadFile( "tests/foreignmeta2.xml", NULL, 0 );
  xmlNode *root = xmlDocGetRootElement( x_doc );
  assertNoThrow( f_doc.set_foreign_metadata( root ) );
  assertNoThrow( f_doc.save( "/tmp/foreignmeta2.out" ) );
  int stat = system( "./tests/foliadiff.sh /tmp/foreignmeta2.out tests/foreignmeta2.out" );
  assertMessage( "/tmp/foreignmeta2.out tests/foreignmeta.out differ!",
		 stat == 0 );
  xmlFreeDoc( x_doc );
}

void sanity_test102( ){
  startTestSerie(" Add a word at wrong position " );
  FoliaElement *p = sanityDoc["WR-P-E-J-0000000001.p.1.s.2.w.7"];
  assertThrow( p->addWord("text='Ahoi'" ), ValueError );

}

void sanity_test102a(){
  startTestSerie(" Declarations - Default set " );
  string xml = "<?xml version=\"1.0\"?>\n"
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\" "
"xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example\" generator=\"libfolia-v0.8\" version=\"0.8\">\n"
"  <metadata type=\"native\">\n"
"    <annotations>\n"
"      <gap-annotation annotator=\"sloot\" set=\"gap-set\"/>\n"
"    </annotations>\n"
"  </metadata>\n"
"  <text xml:id=\"example.text.1\">\n"
"    <gap class=\"X\" />\n"
"  </text>\n"
"</FoLiA>\n" ;

  Document doc;
  assertNoThrow( doc.readFromString(xml) );
  assertTrue( doc["example.text.1"]->select<Gap>()[0]->sett() == "gap-set" );
  assertThrow( doc.declare( AnnotationType::TOKEN,
			    "some-set",
			    "annotatorname='proycon'" ), XmlError );

}

void sanity_test102b(){
  startTestSerie(" Declarations - Set mismatching" );
  string xml = "<?xml version=\"1.0\"?>\n"
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\" "
"xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example\" generator=\"libfolia-v0.8\" version=\"0.8\">\n"
"  <metadata type=\"native\">\n"
"    <annotations>\n"
"      <gap-annotation annotator=\"sloot\" set=\"gap-set\"/>\n"
"    </annotations>\n"
"  </metadata>\n"
"  <text xml:id=\"example.text.1\">\n"
"    <gap class=\"X\" set=\"gip-set\"/>\n"
"  </text>\n"
"</FoLiA>\n" ;

  Document doc;
  assertThrow( doc.readFromString(xml), XmlError );

}

void sanity_test102c(){
  startTestSerie(" Declarations - Multiple sets for the same annotation type " );
  string xml = "<?xml version=\"1.0\"?>\n"
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\" "
"xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example\" generator=\"libfolia-v0.8\" version=\"0.8\">\n"
"  <metadata type=\"native\">\n"
"    <annotations>\n"
"      <gap-annotation annotator=\"sloot\" set=\"extended-gap-set\"/>\n"
"      <gap-annotation annotator=\"sloot\" set=\"gap-set\"/>\n"
"    </annotations>\n"
"  </metadata>\n"
"  <text xml:id=\"example.text.1\">\n"
"    <gap class=\"X\" set=\"gap-set\"/>\n"
"    <gap class=\"Y\" set=\"extended-gap-set\"/>\n"
"  </text>\n"
"</FoLiA>\n" ;

  Document doc;
  assertNoThrow( doc.readFromString(xml) );
  assertTrue( doc["example.text.1"]->select<Gap>()[0]->sett() == "gap-set" );
  assertTrue( doc["example.text.1"]->select<Gap>()[1]->sett() == "extended-gap-set" );

}

void sanity_test102d1(){
  startTestSerie(" Declarations - Multiple sets for the same annotation type (testing failure)" );
  string xml = "<?xml version=\"1.0\"?>\n"
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\" "
"xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example\" generator=\"libfolia-v0.8\" version=\"0.8\">\n"
"  <metadata type=\"native\">\n"
"    <annotations>\n"
"      <gap-annotation annotator=\"sloot\" set=\"extended-gap-set\"/>\n"
"      <gap-annotation annotator=\"sloot\" set=\"gap-set\"/>\n"
"    </annotations>\n"
"  </metadata>\n"
"  <text xml:id=\"example.text.1\">\n"
"    <gap class=\"X\" set=\"gap-set\"/>\n"
"    <gap class=\"Y\"/>\n"
"  </text>\n"
"</FoLiA>\n" ;

  Document doc;
  assertThrow( doc.readFromString(xml), XmlError );

}

void sanity_test102d2(){
  startTestSerie(" Declarations - Multiple sets for the same annotation type (testing failure)" );
  string xml = "<?xml version=\"1.0\"?>\n"
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\" "
"xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example\" generator=\"libfolia-v0.8\" version=\"0.8\">\n"
"  <metadata type=\"native\">\n"
"    <annotations>\n"
"      <gap-annotation annotator=\"sloot\" set=\"extended-gap-set\"/>\n"
"      <gap-annotation annotator=\"sloot\" set=\"gap-set\"/>\n"
"    </annotations>\n"
"  </metadata>\n"
"  <text xml:id=\"example.text.1\">\n"
"    <gap class=\"X\" set=\"gap-set\"/>\n"
"    <gap class=\"Y\" set=\"gip-set\"/>\n"
"  </text>\n"
"</FoLiA>\n" ;

  Document doc;
  assertThrow( doc.readFromString(xml), XmlError );

}

void sanity_test102d3(){
  startTestSerie(" Declarations - Ignore Duplicates" );
  string xml = "<?xml version=\"1.0\"?>\n"
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\" "
"xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example\" generator=\"libfolia-v0.9\" version=\"0.8\">\n"
"  <metadata type=\"native\">\n"
"    <annotations>\n"
"      <gap-annotation annotator=\"sloot\" set=\"gap-set\"/>\n"
"      <gap-annotation annotator=\"sloot\" set=\"gap-set\"/>\n"
"    </annotations>\n"
"  </metadata>\n"
"  <text xml:id=\"example.text.1\">\n"
"    <gap class=\"X\" set=\"gap-set\"/>\n"
"  </text>\n"
"</FoLiA>\n" ;

  Document doc;
  assertNoThrow( doc.readFromString(xml) );
  assertTrue( doc.defaultset(AnnotationType::GAP) == "gap-set" );
  assertTrue( doc.defaultannotator(AnnotationType::GAP) == "sloot" );
  assertTrue( doc.defaultannotator(AnnotationType::GAP,"gap-set") == "sloot" );

}

void sanity_test102e(){
  startTestSerie(" Declarations - Missing declaration " );
  string xml = "<?xml version=\"1.0\"?>\n"
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\" "
"xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example\" generator=\"libfolia-v0.8\" version=\"0.8\">\n"
"  <metadata type=\"native\">\n"
"    <annotations>\n"
"    </annotations>\n"
"  </metadata>\n"
"  <text xml:id=\"example.text.1\">\n"
"    <gap class=\"X\" set=\"extended-gap-set\"/>\n"
"  </text>\n"
"</FoLiA>\n" ;

  Document doc;
  assertThrow( doc.readFromString(xml), XmlError );

}

void sanity_test102f(){
  startTestSerie(" Declarations - Declaration not needed " );
  string xml = "<?xml version=\"1.0\"?>\n"
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\" "
"xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example\" generator=\"libfolia-v0.8\" version=\"0.8\">\n"
"  <metadata type=\"native\">\n"
"    <annotations>\n"
"    </annotations>\n"
"  </metadata>\n"
"  <text xml:id=\"example.text.1\">\n"
"    <gap>\n"
"     <desc>test</desc>\n"
"    </gap>\n"
"  </text>\n"
"</FoLiA>\n" ;

  Document doc;
  assertNoThrow( doc.readFromString(xml) );
}

void sanity_test102g(){
  startTestSerie(" Declarations - Empty set " );
  string xml = "<?xml version=\"1.0\"?>\n"
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\" "
"xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example\" generator=\"libfolia-v0.8\" version=\"0.8\">\n"
"  <metadata type=\"native\">\n"
"    <annotations>\n"
"      <gap-annotation annotator=\"sloot\"/>\n"
"    </annotations>\n"
"  </metadata>\n"
"  <text xml:id=\"example.text.1\">\n"
"    <gap class=\"X\">\n"
"     <desc>test1</desc>\n"
"    </gap>\n"
"    <gap set=\"undefined\" class=\"Y\">\n"
"     <desc>test2</desc>\n"
"    </gap>\n"
"  </text>\n"
"</FoLiA>\n" ;

  Document doc;
  assertNoThrow( doc.readFromString(xml) );
  vector<Gap*> v = doc["example.text.1"]->select<Gap>();
  assertTrue( v[0]->description() == "test1" );
  assertTrue( v[0]->sett() == "undefined" );
  assertTrue( v[1]->description() == "test2" );
  assertTrue( v[1]->sett() == "undefined" );
  assertTrue( v[1]->xmlstring() == "<gap xmlns=\"http://ilk.uvt.nl/folia\" class=\"Y\"><desc>test2</desc></gap>" );

}

void sanity_test102h(){
  startTestSerie(" Declarations - Adding a declaration in same set. (other annotator)" );
  string xml = "<?xml version=\"1.0\"?>\n"
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\" "
"xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example\" generator=\"libfolia-v0.8\" version=\"0.8\">\n"
"  <metadata type=\"native\">\n"
"    <annotations>\n"
"      <gap-annotation annotator=\"sloot\" set=\"gap-set\"/>\n"
"    </annotations>\n"
"  </metadata>\n"
"  <text xml:id=\"example.text.1\">\n"
"    <gap class=\"X\" />\n"
"  </text>\n"
"</FoLiA>\n" ;

  Document doc;
  assertNoThrow( doc.readFromString(xml) );
  assertNoThrow( doc.declare( AnnotationType::GAP,
					"gap-set",
					"annotator='proycon'" ) );
  vector<Gap*> v = doc["example.text.1"]->select<Gap>();
  assertTrue( v[0]->xmlstring() == "<gap xmlns=\"http://ilk.uvt.nl/folia\" annotator=\"sloot\" class=\"X\" set=\"gap-set\"/>" );

}

void sanity_test102i(){
  startTestSerie(" Declarations - miscellanious trouble" );
  string xml = "<?xml version=\"1.0\"?>\n"
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\" "
"xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example\" generator=\"libfolia-v0.8\" version=\"0.8\">\n"
"  <metadata type=\"native\">\n"
"    <annotations>\n"
"      <gap-annotation annotator=\"sloot\" set=\"gap1-set\"/>\n"
"      <gap-annotation annotator=\"sloot\" set=\"gap2-set\"/>\n"
"    </annotations>\n"
"  </metadata>\n"
"  <text xml:id=\"example.text.1\">\n"
"    <gap class=\"X\" set=\"gap1-set\"/>\n"
"  </text>\n"
"</FoLiA>\n" ;

  Document doc;
  assertNoThrow( doc.readFromString(xml) );
  assertTrue( doc.defaultannotator(AnnotationType::GAP,"gap1-set") == "sloot" );
  assertNoThrow( doc.declare( AnnotationType::GAP,
					"gap1-set",
					"annotator='proycon'" ) );
  assertTrue( doc.defaultannotator(AnnotationType::GAP,"gap1-set") == "" );
  assertTrue( doc.defaultannotator(AnnotationType::GAP,"gap2-set") == "sloot" );
  FoliaElement *text = doc["example.text.1"];
  KWargs args = getArgs( "set='gap1-set', class='Y', annotator='proycon'" );
  FoliaElement *g = 0;
  assertNoThrow( g = new Gap( args, &doc ) );
  assertNoThrow( text->append( g ) );
  args = getArgs( "set='gap1-set', class='Z1'" );
  assertNoThrow( g = new Gap( args, &doc ) );
  assertNoThrow( text->append( g ) );
  args = getArgs( "set='gap2-set', class='Z2'" );
  assertNoThrow( g = new Gap( args, &doc ) );
  assertNoThrow( text->append( g ) );
  args = getArgs( "set='gap2-set', class='Y2', annotator='onbekend'" );
  assertNoThrow( g = new Gap( args, &doc ) );
  assertNoThrow( text->append( g ) );
  vector<Gap*> v = doc["example.text.1"]->select<Gap>();
  assertTrue( v[0]->xmlstring() == "<gap xmlns=\"http://ilk.uvt.nl/folia\" annotator=\"sloot\" class=\"X\" set=\"gap1-set\"/>" );
  assertTrue( v[1]->xmlstring() == "<gap xmlns=\"http://ilk.uvt.nl/folia\" annotator=\"proycon\" class=\"Y\" set=\"gap1-set\"/>" );
  assertTrue( v[2]->xmlstring() == "<gap xmlns=\"http://ilk.uvt.nl/folia\" class=\"Z1\" set=\"gap1-set\"/>" );
  assertTrue( v[3]->xmlstring() == "<gap xmlns=\"http://ilk.uvt.nl/folia\" class=\"Z2\" set=\"gap2-set\"/>" );
  assertTrue( v[4]->xmlstring() == "<gap xmlns=\"http://ilk.uvt.nl/folia\" annotator=\"onbekend\" class=\"Y2\" set=\"gap2-set\"/>" );

}

void sanity_test102j(){
  startTestSerie(" Declarations - Adding a declaration in other set." );
  string xml = "<?xml version=\"1.0\"?>\n"
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\" "
"xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example\" generator=\"libfolia-v0.8\" version=\"0.8\">\n"
"  <metadata type=\"native\">\n"
"    <annotations>\n"
"      <gap-annotation annotator=\"sloot\" set=\"gap-set\"/>\n"
"    </annotations>\n"
"  </metadata>\n"
"  <text xml:id=\"example.text.1\">\n"
"    <gap class=\"X\" />\n"
"  </text>\n"
"</FoLiA>\n" ;

  Document doc;
  assertNoThrow( doc.readFromString(xml) );
  FoliaElement *text = doc["example.text.1"];
  assertNoThrow( doc.declare( AnnotationType::GAP,
					"other-set",
					"annotator='proycon'" ) );
  KWargs args = getArgs( "set='other-set', class='Y', annotator='proycon'" );
  FoliaElement *g = 0;
  assertNoThrow( g = new Gap( args,  &doc ) );
  text->append( g );
  args = getArgs( "set='other-set', class='Z'" );
  assertNoThrow( g = new Gap( args, &doc ) );
  text->append( g );
  vector<Gap*> v = text->select<Gap>();
  assertTrue( v[0]->xmlstring() == "<gap xmlns=\"http://ilk.uvt.nl/folia\" class=\"X\" set=\"gap-set\"/>" );
  assertTrue( v[1]->xmlstring() == "<gap xmlns=\"http://ilk.uvt.nl/folia\" class=\"Y\" set=\"other-set\"/>" );
  assertTrue( v[2]->xmlstring() == "<gap xmlns=\"http://ilk.uvt.nl/folia\" class=\"Z\" set=\"other-set\"/>" );

}

void sanity_test102k(){
  startTestSerie(" Declarations - Several annotator types." );
  string xml = "<?xml version=\"1.0\"?>\n"
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\" "
"xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example\" generator=\"libfolia-v0.8\" version=\"0.8\">\n"
"  <metadata type=\"native\">\n"
"    <annotations>\n"
"      <gap-annotation annotatortype=\"auto\" set=\"gap-set\"/>\n"
"    </annotations>\n"
"  </metadata>\n"
"  <text xml:id=\"example.text.1\">\n"
"    <gap class=\"X\" />\n"
"  </text>\n"
"</FoLiA>\n" ;

  Document doc;
  assertNoThrow( doc.readFromString(xml) );
  FoliaElement *text = doc["example.text.1"];
  assertTrue( doc.defaultannotatortype(AnnotationType::GAP) == "auto" );
  vector<Gap*> v = text->select<Gap>();
  assertTrue( v[0]->xmlstring() == "<gap xmlns=\"http://ilk.uvt.nl/folia\" class=\"X\"/>" );
  assertNoThrow( doc.declare( AnnotationType::GAP,
					"gap-set",
					"annotatortype='manual'" ) );
  assertTrue( doc.defaultannotatortype(AnnotationType::GAP) == "" );
  KWargs args = getArgs( "set='gap-set', class='Y', annotatortype='unknown'" );
  FoliaElement *g = 0;
  assertThrow( g = new Gap( args, &doc ), ValueError );
  args = getArgs( "set='gap-set', class='Y', annotatortype='manual'" );
  assertNoThrow( g = new Gap( args, &doc ) );
  text->append( g );
  args = getArgs( "set='gap-set', class='Z', annotatortype='auto'" );
  assertNoThrow( g = new Gap( args, &doc ) );
  text->append( g );
  v = text->select<Gap>();
  assertTrue( v[0]->xmlstring() == "<gap xmlns=\"http://ilk.uvt.nl/folia\" annotatortype=\"auto\" class=\"X\" set=\"gap-set\"/>" );
  assertTrue( v[1]->xmlstring() == "<gap xmlns=\"http://ilk.uvt.nl/folia\" annotatortype=\"manual\" class=\"Y\" set=\"gap-set\"/>" );
  assertTrue( v[2]->xmlstring() == "<gap xmlns=\"http://ilk.uvt.nl/folia\" annotatortype=\"auto\" class=\"Z\" set=\"gap-set\"/>" );

}

void sanity_test102l(){
  startTestSerie(" Declarations - Datetime default." );
  string xml = "<?xml version=\"1.0\"?>\n"
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\" "
"xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example\" generator=\"libfolia-v0.8\" version=\"0.8\">\n"
"  <metadata type=\"native\">\n"
"    <annotations>\n"
"      <gap-annotation set=\"gap-set\" datetime=\"2012-06-18T17:49\"/>\n"
"    </annotations>\n"
"  </metadata>\n"
"  <text xml:id=\"example.text.1\">\n"
"    <gap class=\"X\" />\n"
"    <gap class=\"Y\" datetime=\"2012-06-18T17:50\"/>\n"
"  </text>\n"
"</FoLiA>\n" ;

  Document doc;
  assertNoThrow( doc.readFromString(xml) );
  FoliaElement *text = doc["example.text.1"];
  assertTrue( doc.defaultdatetime(AnnotationType::GAP,"gap-set") == "2012-06-18T17:49:00" );
  vector<Gap*> v = text->select<Gap>();
  assertTrue( v[0]->xmlstring() == "<gap xmlns=\"http://ilk.uvt.nl/folia\" class=\"X\"/>" );
  assertTrue( v[1]->xmlstring() == "<gap xmlns=\"http://ilk.uvt.nl/folia\" class=\"Y\" datetime=\"2012-06-18T17:50:00\"/>" );

}

void sanity_test102m(){
  startTestSerie(" Declarations - removing." );
  string xml = "<?xml version=\"1.0\"?>\n"
    " <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\" "
    "xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example\" generator=\"libfolia-v0.8\" version=\"0.8\">\n"
    "  <metadata type=\"native\">\n"
    "    <annotations>\n"
    "      <gap-annotation set=\"gap-set\" datetime=\"2012-06-18T17:49\"/>\n"
    "    </annotations>\n"
    "  </metadata>\n"
    "  <text xml:id=\"example.text.1\">\n"
    "    <gap class=\"X\" />\n"
    "    <gap class=\"Y\" datetime=\"2012-06-18T17:50\"/>\n"
    "  </text>\n"
    "</FoLiA>\n" ;

  Document doc;
  assertNoThrow( doc.readFromString(xml) );
  assertNoThrow( doc.declare( AnnotationType::GAP,
			      "gap-set2",
			      "annotatortype='manual'" ) );
  assertNoThrow( doc.save( "/tmp/declared1.out" ) );
  assertNoThrow( doc.un_declare( AnnotationType::GAP, "gap-set2" ) );
  assertNoThrow( doc.save( "/tmp/declared2.out" ) );
  assertThrow( doc.un_declare( AnnotationType::GAP, "gap-set" ), XmlError );
  assertNoThrow( doc.save( "/tmp/declared3.out" ) );
}

void sanity_test102n(){
  startTestSerie(" Declarations - using an alias." );
  string xml = "<?xml version=\"1.0\"?>\n"
    " <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\" "
    "xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example\" generator=\"libfolia-v0.8\" version=\"0.8\">\n"
    "  <metadata type=\"native\">\n"
    "    <annotations>\n"
    "      <gap-annotation set=\"some very convoluted url or such which clutters all\" alias=\"gap-set\" datetime=\"2012-06-18T17:49\"/>\n"
    "      <division-annotation set=\"a long div annotation name\" alias=\"div-set\" datetime=\"2012-06-18T17:49\"/>\n"
    "    </annotations>\n"
    "  </metadata>\n"
    "  <text xml:id=\"example.text.1\">\n"
    "    <gap class=\"X\" />\n"
    "    <gap class=\"Y\" datetime=\"2012-06-18T17:50\"/>\n"
    "  </text>\n"
    "</FoLiA>\n" ;

  Document doc;
  assertNoThrow( doc.readFromString(xml) );
  assertNoThrow( doc.declare( AnnotationType::GAP,
			      "nog zon ingewikkelde en veels te lange declaratie",
			      "annotatortype='manual', alias='gap-set2'" ) );
  assertNoThrow( doc.save( "/tmp/declared1.out" ) );
  // using a setname which is already an alias is a error
  assertThrow( doc.declare( AnnotationType::GAP, "gap-set2", "alias='gap-set3'"),
	       XmlError );
  // using an alias  which is already an alias is a error
  assertThrow( doc.declare( AnnotationType::GAP, "someset", "alias='gap-set'"),
	       XmlError );
  // using an alias  which is already a setname is a error
  assertThrow( doc.declare( AnnotationType::GAP, "some_other_set", "alias='nog zon ingewikkelde en veels te lange declaratie'"),
	       XmlError );
  // just declaring again is NOT an error!
  assertNoThrow( doc.declare( AnnotationType::GAP,
			      "nog zon ingewikkelde en veels te lange declaratie",
			      "annotatortype='manual', alias='gap-set2'" ) );
  // declaring again with another alias IS an error!
  assertThrow( doc.declare( AnnotationType::GAP,
			    "nog zon ingewikkelde en veels te lange declaratie",
			    "annotatortype='manual', alias='gap-set3'" ),
	       XmlError) ;
  // declaring again with same alias and another setname IS an error!
  assertThrow( doc.declare( AnnotationType::GAP,
			    "niet zon ingewikkelde en veels te lange declaratie",
			    "annotatortype='manual', alias='gap-set2'" ),
	       XmlError );
  assertNoThrow( doc.un_declare( AnnotationType::GAP, "gap-set2" ) );
  assertNoThrow( doc.save( "/tmp/declared2.out" ) );
  assertThrow( doc.un_declare( AnnotationType::GAP, "gap-set" ), XmlError );
  assertNoThrow( doc.save( "/tmp/declared3.out" ) );
}

void sanity_test102o(){
  startTestSerie(" Declarations - using same sets and aliases for different types." );
  Document doc;
  assertNoThrow( doc.readFromFile("tests/aliases.xml") );
  assertNoThrow( doc.save( "/tmp/aliases.xml" ) );
  int stat = system( "./tests/foliadiff.sh /tmp/aliases.xml tests/aliases.xml" );
  assertMessage( "/tmp/aliases.xml tests/aliases.xml differ!",
   		 (stat == 0) );

}

void sanity_test103( ){
  startTestSerie(" Alien namespaces - Checking whether properly ignored " );
  string xml = "<?xml version=\"1.0\"?>\n"
    " <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\" "
    "xmlns=\"http://ilk.uvt.nl/folia\" xmlns:alien=\"http://somewhere.else\" xml:id=\"example\" generator=\"libfolia-v0.8\" version=\"0.8\">\n"
    "  <metadata type=\"native\">\n"
    "    <annotations/>\n"
    "  </metadata>\n"
    "  <text xml:id=\"example.text.1\">\n"
    "    <s xml:id=\"example.text.1.s.1\">\n"
    "      <alien:blah >\n"
    "        <w xml:id=\"example.text.1.s.1.alienword\">\n"
    "        </w>\n"
    "      </alien:blah >\n"
    "      <w xml:id=\"example.text.1.s.1.w.1\" alien:at=\"alien\">\n"
    "          <t>word</t>\n"
    "          <alien:invasion number=\"99999\" />\n"
    "      </w>\n"
    "    </s>\n"
    "  </text>\n"
    "</FoLiA>\n" ;
  Document doc;
  assertNoThrow( doc.readFromString(xml) );
  assertTrue( len(doc.words()) == 1 ); // first word is in alien namespace
  // not read
  FoliaElement *w = doc["example.text.1.s.1.alienword"];
  assertTrue( w == 0 );   // doesn't exist
  w = doc["example.text.1.s.1.w.1"];
  assertTrue( w->xmlstring() == "<w xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example.text.1.s.1.w.1\"><t>word</t></w>" );

}

void sanity_test104a( ){
  startTestSerie(" Sanity Check - Speech data (without attributes)" );
  string xml = "<?xml version=\"1.0\"?>\n"
    "<FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\" xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example\" generator=\"manual\" version=\"0.12\">\n"
    "  <metadata type=\"native\">\n"
    "    <annotations>\n"
    "      <utterance-annotation set=\"utterances\" />\n"
    "    </annotations>\n"
    "  </metadata>\n"
    "  <speech xml:id=\"example.speech\">\n"
    "    <utt xml:id=\"example.speech.utt.1\">\n"
    "      <ph>həlˈəʊ wˈɜːld</ph>\n"
    "    </utt>\n"
    "    <utt xml:id=\"example.speech.utt.2\">\n"
    "      <w xml:id=\"example.speech.utt.2.w.1\">\n"
    "        <ph>həlˈəʊ</ph>\n"
    "      </w>\n"
    "      <w xml:id=\"example.speech.utt.2.w.2\">\n"
    "        <ph>wˈɜːld</ph>\n"
    "      </w>\n"
    "    </utt>\n"
    "  </speech>\n"
    "</FoLiA>\n";
  Document doc;
  assertNoThrow( doc.readFromString(xml) );
  //  assertNoThrow( doc.save( "/tmp/test104a.xml" ) );
  //  assertTrue( isinstance(doc.doc(0), Speech_t) );
  assertTrue( isinstance(doc["example.speech.utt.1"], Utterance_t) );
  assertEqual( doc["example.speech.utt.1"]->phon(), "həlˈəʊ wˈɜːld" );
  assertThrow( doc["example.speech.utt.1"]->text(), NoSuchText ); // doesn't exist
  assertEqual( doc["example.speech.utt.2"]->phon(), "həlˈəʊ wˈɜːld" );
  // the str() function should return phon() for PhonContent
  assertEqual( doc["example.speech.utt.2"]->str(), "həlˈəʊ wˈɜːld" );
}

void sanity_test104b( ){
  startTestSerie(" Sanity Check - Speech data (with speech attributes)" );
  string xml="<?xml version=\"1.0\"?>\n"
    "<FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\" xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example\" generator=\"manual\" version=\"0.12\">\n"
    "  <metadata type=\"native\">\n"
    "    <annotations>\n"
    "      <utterance-annotation set=\"utterances\" />\n"
    "    </annotations>\n"
    "  </metadata>\n"
    "  <speech xml:id=\"example.speech\" src=\"helloworld.ogg\" speaker=\"proycon\">\n"
    "    <utt xml:id=\"example.speech.utt.1\" begintime=\"00:00:00\" endtime=\"00:00:02.012\">\n"
    "      <ph>həlˈəʊ wˈɜːld</ph>\n"
    "    </utt>\n"
    "    <utt xml:id=\"example.speech.utt.2\">\n"
    "      <w xml:id=\"example.speech.utt.2.w.1\" begintime=\"00:00:00\" endtime=\"00:00:01\">\n"
    "        <ph>həlˈəʊ</ph>\n"
    "      </w>\n"
    "      <w xml:id=\"example.speech.utt.2.w.2\" begintime=\"00:00:01.267\" endtime=\"00:00:02.012\">\n"
    "        <ph>wˈɜːld</ph>\n"
    "      </w>\n"
    "    </utt>\n"
    "  </speech>\n"
    "</FoLiA>\n";

  Document doc;
  assertNoThrow( doc.readFromString(xml) );
  assertNoThrow( doc.save( "/tmp/test104b.xml" ) );
  //assertTrue( isinstance(doc.doc(), folia.Speech) );
  assertTrue( isinstance(doc["example.speech.utt.1"], Utterance_t) );
  assertEqual( doc["example.speech.utt.1"]->phon(), "həlˈəʊ wˈɜːld" );
  assertThrow( doc["example.speech.utt.1"]->text(), NoSuchText ); // doesn't exist
  assertEqual( doc["example.speech.utt.2"]->phon(), "həlˈəʊ wˈɜːld" );
  assertEqual( doc["example.speech"]->speech_speaker(), "proycon" );
  assertEqual( doc["example.speech"]->speech_src(), "helloworld.ogg" );
  assertEqual( doc["example.speech.utt.1"]->begintime(), "00:00:00.000" );
  assertEqual( doc["example.speech.utt.1"]->endtime(), "00:00:02.012" );
  assertEqual( doc["example.speech.utt.2.w.2"]->speech_speaker(), "proycon" );
  assertEqual( doc["example.speech.utt.2.w.2"]->speech_src(), "helloworld.ogg" );
  assertEqual( doc["example.speech.utt.2.w.2"]->begintime(), "00:00:01.267" );
  assertEqual( doc["example.speech.utt.2.w.2"]->endtime(), "00:00:02.012" );
}

void sanity_test105(){
  startTestSerie( "Sanity Check - Complex alignment" );
  string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<FoLiA xmlns=\"http://ilk.uvt.nl/folia\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xml:id=\"test\" version=\"0.8\" generator=\"libfolia-v0.4\">\n"
    "  <metadata type=\"native\">\n"
    "    <annotations>\n"
    "      <complexalignment-annotation />\n"
    "      <alignment-annotation set=\"blah\" />\n"
    "    </annotations>\n"
    "  </metadata>\n"
    "  <text xml:id=\"test.text\">\n"
    "    <p xml:id=\"p.1\">\n"
    "      <s xml:id=\"p.1.s.1\"><t>Dit is een test.</t></s>\n"
    "      <s xml:id=\"p.1.s.2\"><t>Ik wil kijken of het werkt.</t></s>\n"
    "      <complexalignments>\n"
    "        <complexalignment>\n"
    "          <alignment>\n"
    "            <aref id=\"p.1.s.1\" type=\"s\" />\n"
    "            <aref id=\"p.1.s.2\" type=\"s\" />\n"
    "	       </alignment>\n"
    "          <alignment class=\"translation\" xlink:href=\"en.folia.xml\" xlink:type=\"simple\">\n"
    "            <aref id=\"p.1.s.1\" type=\"s\" />\n"
    "	       </alignment>\n"
    "        </complexalignment>\n"
    "      </complexalignments>\n"
    "    </p>\n"
    "  </text>\n"
    "</FoLiA>\n";
  Document doc;
  assertNoThrow( doc.readFromString(xml) );
  //        self.assertTrue(doc.xml() is not None) #serialisation check
  vector<Paragraph*> pargs;
  assertNoThrow( pargs = doc.paragraphs() );
  vector<ComplexAlignmentLayer*> cal = pargs[0]->annotations<ComplexAlignmentLayer>();
  assertEqual( len(cal),1);
  vector<ComplexAlignment*> ca = cal[0]->annotations<ComplexAlignment>();
  assertEqual( len(ca),1);
  vector<Alignment*> alignments = ca[0]->select<Alignment>();
  assertEqual(len(alignments),2);
}

void sanity_test106( ){
  startTestSerie(" embedded sentences " );
  string xml = "<?xml version=\"1.0\"?>\n"
    " <FoLiA xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"doc\">\n"
    "   <metadata type=\"native\">\n"
    "     <annotations>\n"
    "      <token-annotation annotator=\"ucto\" annotatortype=\"auto\" set=\"tokconfig-nl\"/>\n"
    "     </annotations>\n"
    "   </metadata>\n"
    "  <text xml:id=\"doc.text\">\n"
    "   <p xml:id=\"doc.p.1\">\n"
    "    <s xml:id=\"doc.p.1.s.1\">\n"
    "     <w xml:id=\"doc.p.1.s.1.w.1\" class=\"WORD\">\n"
    "      <t>Hij</t>\n"
    "     </w>\n"
    "     <w xml:id=\"doc.p.1.s.1.w.2\" class=\"WORD\" space=\"no\">\n"
    "      <t>zegt</t>\n"
    "     </w>\n"
    "     <w xml:id=\"doc.p.1.s.1.w.3\" class=\"PUNCTUATION\">\n"
    "      <t>:</t>\n"
    "     </w>\n"
    "     <w xml:id=\"doc.p.1.s.1.w.4\" class=\"PUNCTUATION\" space=\"no\">\n"
    "      <t>\"</t>\n"
    "     </w>\n"
    "     <quote xml:id=\"doc.p.1.s.1.quote.1\">\n"
    "      <s xml:id=\"doc.p.1.s.1.quote.1.s.1\" auth=\"no\">\n"
    "       <w xml:id=\"doc.p.1.s.1.quote.1.s.1.w.1\" class=\"WORD\">\n"
    "          <t>Dit</t>\n"
    "        </w>\n"
    "        <w xml:id=\"doc.p.1.s.1.quote.1.s.1.w.2\" class=\"WORD\">\n"
    "          <t>is</t>\n"
    "        </w>\n"
    "        <w xml:id=\"doc.p.1.s.1.quote.1.s.1.w.3\" class=\"WORD\">\n"
    "          <t>de</t>\n"
    "        </w>\n"
    "        <w xml:id=\"doc.p.1.s.1.quote.1.s.1.w.4\" class=\"WORD\">\n"
    "          <t>eerste</t>\n"
    "        </w>\n"
    "        <w xml:id=\"doc.p.1.s.1.quote.1.s.1.w.5\" class=\"WORD\" space=\"no\">\n"
    "          <t>zin</t>\n"
    "        </w>\n"
    "        <w xml:id=\"doc.p.1.s.1.quote.1.s.1.w.6\" class=\"PUNCTUATION\">\n"
    "          <t>.</t>\n"
    "        </w>\n"
    "      </s>\n"
    "      <s xml:id=\"doc.p.1.s.1.quote.1.s.2\" auth=\"no\">\n"
    "        <w xml:id=\"doc.p.1.s.1.quote.1.s.2.w.1\" class=\"WORD\">\n"
    "          <t>Dit</t>\n"
    "        </w>\n"
    "        <w xml:id=\"doc.p.1.s.1.quote.1.s.2.w.2\" class=\"WORD\">\n"
    "          <t>is</t>\n"
    "        </w>\n"
    "        <w xml:id=\"doc.p.1.s.1.quote.1.s.2.w.3\" class=\"WORD\">\n"
    "          <t>de</t>\n"
    "        </w>\n"
    "        <w xml:id=\"doc.p.1.s.1.quote.1.s.2.w.4\" class=\"WORD\">\n"
    "          <t>tweede</t>\n"
    "        </w>\n"
    "        <w xml:id=\"doc.p.1.s.1.quote.1.s.2.w.5\" class=\"WORD\" space=\"no\">\n"
    "          <t>zin</t>\n"
    "        </w>\n"
    "        <w xml:id=\"doc.p.1.s.1.quote.1.s.2.w.6\" class=\"PUNCTUATION\" space=\"no\">\n"
    "          <t>.</t>\n"
    "        </w>\n"
    "      </s>\n"
    "     </quote>\n"
    "     <w xml:id=\"doc.p.1.s.1.w.5\" class=\"PUNCTUATION\" space=\"no\">\n"
    "      <t>\"</t>\n"
    "     </w>\n"
    "     <w xml:id=\"doc.p.1.s.1.w.6\" class=\"PUNCTUATION\">\n"
    "      <t>,</t>\n"
    "     </w>\n"
    "     <w xml:id=\"doc.p.1.s.1.w.7\" class=\"WORD\">\n"
    "      <t>en</t>\n"
    "     </w>\n"
    "     <w xml:id=\"doc.p.1.s.1.w.8\" class=\"WORD\">\n"
    "      <t>loopt</t>\n"
    "     </w>\n"
    "     <w xml:id=\"doc.p.1.s.1.w.9\" class=\"WORD\" space=\"no\">\n"
    "      <t>verder</t>\n"
    "     </w>\n"
    "     <w xml:id=\"doc.p.1.s.1.w.10\" class=\"PUNCTUATION\">\n"
    "      <t>.</t>\n"
    "     </w>\n"
    "    </s>\n"
    "   </p>\n"
    "  </text>\n"
    " </FoLiA>\n";
  Document doc;
  assertNoThrow( doc.readFromString(xml) );
  assertTrue( len(doc.sentences()) == 1 ); // one sentence at top level
  vector<Sentence*> s = doc.sentenceParts();
  assertTrue( s.size() == 3 );
  vector<Word*> v = s[0]->wordParts();
  assertTrue( v[5]->str() == "doc.p.1.s.1.quote.1.s.2" );
  v = s[1]->wordParts();
  assertTrue( v[3]->str() == "eerste" );
  v = s[2]->wordParts();
  assertTrue( v[3]->str() == "tweede" );
}

void sanity_test107( ){
  startTestSerie("107 Some quoting");
  Document doc;
  assertNoThrow( doc.readFromFile("tests/fg.xml") );
  vector<Sentence *> s = doc.sentenceParts();
  assertTrue( s.size() == 24 );
  vector<Word*> wv = s[18]->wordParts();
  assertTrue( wv[1]->str() == "zegt" );
  assertTrue( wv[6]->str() == "doc.p.1.s.1.quote.1.s.15.quote.1.s.1" );

}

void sanity_test108( ){
  Document doc;
  startTestSerie( " Attributes - invalid values " );
  string xml = "<?xml version=\"1.0\"?>\n"
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\" "
"xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"voorbeeld 1\" generator=\"libfolia-v0.8\" version=\"0.8\">\n"
"</FoLiA>\n" ;
  assertThrow( doc.readFromString(xml), XmlError );
  assertThrow( new Sentence( getArgs("id='dit mag ook niet'"), &doc ),
	       XmlError );
  assertThrow( new Sentence( getArgs("id='1.ook.niet'"), &doc ), XmlError );
  assertThrow( new Sentence( getArgs("id='dit:ook:niet'"), &doc ), XmlError );
}

void sanity_test109( ){
  startTestSerie( " type hierarchy " );
  assertTrue( isSubClass( PlaceHolder_t, AbstractStructureElement_t ) );
  assertTrue( ( isSubClass<PlaceHolder, AbstractStructureElement>() ) );
  assertTrue( isSubClass( PlaceHolder_t, Word_t ) );
  assertTrue( ( isSubClass<PlaceHolder, Word>() ) );
  assertTrue( isSubClass( PosAnnotation_t, AbstractTokenAnnotation_t ) );
  assertTrue( ( isSubClass<PosAnnotation, AbstractTokenAnnotation>() ) );
}

void sanity_test110(){
  startTestSerie( " Submetadata " );
  Document doc( "file='tests/example.xml'" );
  auto sm = doc["sandbox.3"]->getmetadata();
  assertEqual( sm->datatype(), "NativeMetaData" );
  KWargs att = sm->get_avs();
  assertEqual( att["author"], "proycon" );
  sm = doc["example.table.1.w.1"]->getmetadata();
  assertEqual( sm->datatype(), "NativeMetaData" );
  att = sm->get_avs();
  assertEqual( att["author"], "proycon" );
  string val = doc["example.table.1.w.1"]->getmetadata("author");
  assertEqual( val, "proycon" );
  sm = doc["WR-P-E-J-0000000001.div0.1"]->getmetadata();
  assertEqual( sm->datatype(), "NativeMetaData" );
  att = sm->get_avs();
  assertEqual( att["originalsource"], "https://nl.wikipedia.org/wiki/Stemma" );
  sm = doc["WR-P-E-J-0000000001.p.1.s.1.w.1"]->getmetadata();
  assertEqual( sm->datatype(), "NativeMetaData" );
  att = sm->get_avs();
  assertEqual( att["originalsource"], "https://nl.wikipedia.org/wiki/Stemma" );
  val = doc["WR-P-E-J-0000000001.div0.1"]->getmetadata("originalsource");
  assertEqual( val, "https://nl.wikipedia.org/wiki/Stemma" );
  sm = doc["span.correction"]->getmetadata();
  assertEqual( sm->datatype(), "ExternalMetaData" );
  assertEqual( sm->src(), "blaat" );
  sm = doc["bibhead"]->getmetadata();
  assertEqual( sm->datatype(), "ForeignMetaData" );
  vector<FoliaElement*> fv = sm->get_foreigners();
  string cont = fv[0]->xmlstring();
  assertEqual( cont, "<foreign-data xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"ergens\">\n"
"        <fd:node xmlns:fd=\"foreigns\">\n"
"	  <fd:sub att=\"1\">test</fd:sub>\n"
"	</fd:node>\n"
"      </foreign-data>" );
}

void sanity_test120( ){
  startTestSerie( " Word References - Forward Must fail " );
  Document d;
  string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<FoLiA xmlns=\"http://ilk.uvt.nl/folia\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xml:id=\"doc\" version=\"1.5\">\n"
    "  <metadata type=\"native\">\n"
    "    <annotations>\n"
    "      <entity-annotation/>\n"
    "    </annotations>\n"
    "  </metadata>\n"
    "  <text xml:id=\"text\">\n"
    "    <s xml:id=\"s1\">\n"
    "      <entities>\n"
    "	     <entity xml:id=\"e1\">\n"
    "	       <wref id=\"w1\"/>\n"
    "	       <wref id=\"w2\"/>\n"
    "	     </entity>\n"
    "      </entities>\n"
    "      <w xml:id=\"w1\">\n"
    " 	     <t>Woord1</t>\n"
    "      </w>\n"
    "      <w xml:id=\"w2\">\n"
    "	     <t>Woord2</t>\n"
    "      </w>\n"
    "    </s>\n"
    "  </text>\n"
    "</FoLiA>\n";
  assertThrow( d.readFromString( xml ), XmlError );
}

void sanity_test121( ){
  startTestSerie( " Word References - backward with wrong t NOT DETECTED!" );
  Document d;
  string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<FoLiA xmlns=\"http://ilk.uvt.nl/folia\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xml:id=\"doc\" version=\"1.5\">\n"
    "  <metadata type=\"native\">\n"
    "    <annotations>\n"
    "      <entity-annotation/>\n"
    "    </annotations>\n"
    "  </metadata>\n"
    "  <text xml:id=\"text\">\n"
    "    <s xml:id=\"s1\">\n"
    "      <w xml:id=\"w1\">\n"
    " 	     <t>Woord1</t>\n"
    "      </w>\n"
    "      <w xml:id=\"w2\">\n"
    "	     <t>Woord2</t>\n"
    "      </w>\n"
    "      <entities>\n"
    "	     <entity xml:id=\"e1\">\n"
    "	       <wref id=\"w1\" t=\"Woord1\"/>\n"
    "	       <wref id=\"w2\" t=\"FOUT\"/>\n"
    "	     </entity>\n"
    "      </entities>\n"
    "    </s>\n"
    "  </text>\n"
    "</FoLiA>\n";

  //  assertThrow( d.readFromString( xml ), XmlError );
  assertNoThrow( d.readFromString( xml ) );
}

void sanity_test122( ){
  startTestSerie( " Word References - refering a non-word " );
  Document d;
  string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<FoLiA xmlns=\"http://ilk.uvt.nl/folia\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xml:id=\"doc\" version=\"1.5\">\n"
    "  <metadata type=\"native\">\n"
    "    <annotations>\n"
    "      <entity-annotation/>\n"
    "      <lemma-annotation set=\"lem\"/>\n"
    "    </annotations>\n"
    "  </metadata>\n"
    "  <text xml:id=\"text\">\n"
    "    <s xml:id=\"s1\">\n"
    "      <w xml:id=\"w1\">\n"
    "	     <t>Woord1</t>\n"
    " 	     <lemma id=\"l1\" class=\"woord\"/>\n"
    "      </w>\n"
    "      <w xml:id=\"w2\">\n"
    "	     <t>Woord2</t>\n"
    "      </w>\n"
    "      <entities>\n"
    "	     <entity xml:id=\"e1\">\n"
    "	       <wref id=\"l1\" t=\"woord\"/>\n"
    "	       <wref id=\"w2\" t=\"Woord1\"/>\n"
    "	     </entity>\n"
    "      </entities>\n"
    "    </s>\n"
    "  </text>\n"
    "</FoLiA>\n";

  assertThrow( d.readFromString( xml ), XmlError );
}

#if FOLIA_INT_VERSION < 116
void sanity_test123( ){
  startTestSerie( " Layers - using Word not Reference " );
  Document d;
  assertNoThrow( d.readFromFile( "tests/example_2.xml") );
}
#else
void sanity_test123( ){
  startTestSerie( " Layers - using Word not Reference " );
  Document d;
  assertThrow( d.readFromFile( "tests/example_2.xml" ), XmlError );
}
#endif

void edit_test001a( ){
  startTestSerie( " Add a sentence to the first paragraph ");
  FoliaElement *p = 0;
  // grab first paragraph
  Document editDoc( "file='tests/example.xml'" );
  assertNoThrow( p = editDoc.paragraphs(0) );
  size_t tmp = p->size();
  // add a sentence
  FoliaElement *s = 0;
  assertNoThrow( s = new Sentence( getArgs("generate_id='" + p->id() + "'"),
				   &editDoc ) );
  p->append( s );
  // add words to the sentence
  Word *w = 0;
  assertNoThrow( w = new Word( getArgs("text='Dit', annotator='testscript', annotatortype='auto', generate_id='" + s->id() + "'" ), &editDoc ));
  s->append( w );
  assertNoThrow( w = new Word( getArgs("text='is', annotator='testscript', annotatortype='AUTO', generate_id='" + s->id() + "'" ), &editDoc ));
  s->append( w );
  assertNoThrow( w = new Word( getArgs("text='een', annotator='testscript', annotatortype='auto', generate_id='" + s->id() + "'" ), &editDoc ));
  s->append( w );
  assertNoThrow( w = new Word( getArgs("text='nieuwe', annotator='testscript', annotatortype='auto', generate_id='" + s->id() + "'" ), &editDoc ));
  s->append( w );
  assertNoThrow( w = new Word( getArgs("text='zin', annotator='testscript', annotatortype='auto', generate_id='" + s->id() + "', space='no'" ), &editDoc ));
  s->append( w );
  assertNoThrow( w = new Word( getArgs("text='.', class='PUNCTUATION', annotator='testscript', annotatortype='auto', generate_id='" + s->id() + "'" ),  &editDoc ) );
  s->append( w );
  for ( int i=0; i < 6; ++i ){
    assertTrue( s->index(i)->id() == s->id() + ".w." + TiCC::toString(i+1) );
  }

  // index check
  assertTrue( editDoc[s->id()] == s );
  assertTrue( editDoc[s->id() + ".w.3"] == s->index(2) );

  // attribute check
  assertTrue( s->index(0)->annotator() == "testscript" );
  assertTrue( s->index(2)->annotatortype() == AUTO );

  // adition to paragraph correct?
  assertEqual( p->size(), (tmp+1) );
  assertTrue( p->rindex(0) == s );

  // last word ok?
  assertTrue( w->cls() == "PUNCTUATION" );
  assertTrue( w->text() == "." );

  // sentence ok?
  assertEqual( s->toktext(), "Dit is een nieuwe zin ." );
  assertEqual( s->text(), "Dit is een nieuwe zin." );

  // all well?

  assertEqual( s->xmlstring(), "<s xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.9\"><w xml:id=\"WR-P-E-J-0000000001.p.1.s.9.w.1\" annotator=\"testscript\"><t>Dit</t></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.9.w.2\" annotator=\"testscript\"><t>is</t></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.9.w.3\" annotator=\"testscript\"><t>een</t></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.9.w.4\" annotator=\"testscript\"><t>nieuwe</t></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.9.w.5\" annotator=\"testscript\" space=\"no\"><t>zin</t></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.9.w.6\" annotator=\"testscript\" class=\"PUNCTUATION\"><t>.</t></w></s>" );
}


void edit_test001b( ){
  startTestSerie( " Add a sentence to the first paragraph (shortcuts)" );
  FoliaElement *p = 0;
  Document editDoc( "file='tests/example.xml'" );
  // grab first paragraph
  assertNoThrow( p = editDoc.paragraphs(0) );
  size_t tmp = p->size();
  // add a sentence
  FoliaElement *s = 0;
  assertNoThrow( s = p->addSentence() );
  // add words to the sentence
  Word *w = 0;
  assertNoThrow( w = s->addWord() );
  w->settext("Dit");
  assertNoThrow( w = s->addWord( "textclass='apart'" ) );
  w->settext( "is" );
  w->settext( "was", "apart" );
  assertNoThrow( w = s->addWord( "text='een'" ) );
  KWargs ann;
  ann["text"] = "nieuwe";
  ann["class"] = "WORD";
  assertNoThrow( w = s->addWord( ann ) );
  w->settext( "oude", "apart" );
  ann["text"] = "zin";
  w = 0;
  assertNoThrow( w = s->addWord( ann ) );
  FoliaElement *w2 = 0;
  assertNoThrow( w2 = s->addWord( "text='.', class='PUNCTUATION'" ) );

  assertTrue( len(s->words()) == 6 );
  assertTrue( w->text() == "zin" );
  assertTrue( editDoc[w->id()] == w );

  assertTrue( w2->text() == "." );

  // adition to paragraph correct?
  assertEqual( p->size(), (tmp+1) );
  assertTrue( p->rindex(0) == s );

  // all well?
  assertEqual( s->xmlstring(), "<s xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.9\"><w xml:id=\"WR-P-E-J-0000000001.p.1.s.9.w.1\"><t>Dit</t></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.9.w.2\" textclass=\"apart\"><t>is</t><t class=\"apart\">was</t></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.9.w.3\"><t>een</t></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.9.w.4\" class=\"WORD\"><t>nieuwe</t><t class=\"apart\">oude</t></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.9.w.5\" class=\"WORD\"><t>zin</t></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.9.w.6\" class=\"PUNCTUATION\"><t>.</t></w></s>" );
  assertEqual( s->text() ,"Dit is een nieuwe zin ." );
  assertEqual( s->text("apart"), "was oude" );
  vector<Word*> wv = s->select<Word>();
  wv.erase(remove_if( wv.begin(), wv.end(),
		      [](Word *w) { return w->textclass() != "apart"; }), wv.end());
  assertEqual( wv.size(), 1 );
}

void edit_test002( ){
  startTestSerie( " Add an token annotation (pos, lemma) " );
  Document editDoc( "file='tests/example.xml'" );
  // grab a word (naam)
  FoliaElement *w = 0;
  assertNoThrow( w = editDoc["WR-P-E-J-0000000001.p.1.s.2.w.11"] );

  assertNoThrow( editDoc.declare( AnnotationType::POS,
					"adhocpos") );

  assertNoThrow( editDoc.declare( AnnotationType::LEMMA,
					"adhoclemma") );

  // add a pos annotation (in a different set than the one already present, to prevent conflict)
  KWargs args = getArgs( "set='adhocpos', class='NOUN', annotator='testscript', annotatortype='auto'" );
  assertNoThrow( w->addPosAnnotation( args ) );
  args = getArgs( "set='adhoclemma', class='NAAM', annotator='testscript', annotatortype='auto', datetime='1982-12-15T19:00:01'");
  assertNoThrow( w->addLemmaAnnotation( args ) );

  FoliaElement *p = 0;
  assertNoThrow( p = w->annotation<PosAnnotation>( "adhocpos") );
  assertTrue( p->isinstance<PosAnnotation>() );
  assertTrue( p->cls() == "NOUN" );
  assertTrue( w->pos("adhocpos") == "NOUN" );

  assertNoThrow( p = w->annotation<LemmaAnnotation>( "adhoclemma") );
  assertTrue( p->isinstance<LemmaAnnotation>() );
  assertTrue( p->cls() == "NAAM" );
  assertTrue( w->lemma("adhoclemma") == "NAAM" );
  assertTrue( w->xmlstring() == "<w xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.2.w.11\"><t>naam</t><pos class=\"N(soort,ev,basis,zijd,stan)\" set=\"https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn\"/><lemma class=\"naam\" set=\"lemmas-nl\"/><pos annotator=\"testscript\" annotatortype=\"auto\" class=\"NOUN\" set=\"adhocpos\"/><lemma annotator=\"testscript\" annotatortype=\"auto\" class=\"NAAM\" datetime=\"1982-12-15T19:00:01\" set=\"adhoclemma\"/></w>");

}

void edit_test003( ){
  startTestSerie( " Add an token annotation (pos, lemma) (alternative) " );
  // grab a word (naam)
  Document editDoc( "file='tests/example.xml'" );
  FoliaElement *w = 0;
  assertNoThrow( w = editDoc["WR-P-E-J-0000000001.p.1.s.2.w.11"] );

  assertNoThrow( editDoc.declare( AnnotationType::POS, "adhocpos") );

  assertNoThrow( editDoc.declare( AnnotationType::LEMMA, "adhoclemma") );

  // add a pos annotation (in a different set than the one already present, to prevent conflict)
  KWargs args = getArgs( "set='adhocpos', class='NOUN', annotator='testscript', annotatortype='auto'" );
  assertNoThrow( w->addAnnotation<PosAnnotation>( args ) );
  args = getArgs( "set='adhoclemma', class='NAAM', annotator='testscript', annotatortype='auto'" );
  assertNoThrow( w->addAnnotation<LemmaAnnotation>( args ) );

  FoliaElement *p = 0;
  assertNoThrow( p = w->annotation<PosAnnotation>( "adhocpos") );
  assertTrue( p->isinstance<PosAnnotation>() );
  assertTrue( p->cls() == "NOUN" );

  assertNoThrow( p = w->annotation<LemmaAnnotation>( "adhoclemma") );
  assertTrue( p->isinstance<LemmaAnnotation>() );
  assertTrue( p->cls() == "NAAM" );

  // check the outcome
  assertTrue( w->xmlstring() == "<w xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.2.w.11\"><t>naam</t><pos class=\"N(soort,ev,basis,zijd,stan)\" set=\"https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn\"/><lemma class=\"naam\" set=\"lemmas-nl\"/><pos annotator=\"testscript\" annotatortype=\"auto\" class=\"NOUN\" set=\"adhocpos\"/><lemma annotator=\"testscript\" annotatortype=\"auto\" class=\"NAAM\" set=\"adhoclemma\"/></w>");
}

void edit_test003b( ){
  startTestSerie( " Add an token annotation (pos, lemma) with missing declaration ") ;
  Document editDoc( "file='tests/example.xml'" );
  // grab a word (naam)
  FoliaElement *w = 0;
  assertNoThrow( w = editDoc["WR-P-E-J-0000000001.p.1.s.2.w.11"] );

  assertNoThrow( editDoc.declare( AnnotationType::POS, "adhocpos") );

  // add a pos annotation (in a different set than the one declared,
  // to generate a conflict)
  KWargs args = getArgs( "set='adhopcos', class='NOUN', annotator='testscript', annotatortype='auto'" );
  assertThrow( w->addAnnotation<PosAnnotation>( args ), ValueError );
}

void edit_test004a( ){
  startTestSerie( " Add a token default-set annotation which gives a name clash " );
  Document editDoc( "file='tests/example.xml'" );
  // grab a word (naam)
  FoliaElement *w = 0;
  assertNoThrow( w = editDoc["WR-P-E-J-0000000001.p.1.s.2.w.11"] );
  // add a pos annotation without specifying a set (should take default set)
  KWargs args = getArgs( "class='N', annotator='testscript', annotatortype='auto'" );
  // will add an alternative
  args["set"]="https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn";
  assertNoThrow( w->addPosAnnotation( args ) );
  // will not throw, because an new alternative is created
  assertNoThrow( w->addPosAnnotation( args ) );
  args = getArgs( "class='naam', annotator='testscript', annotatortype='auto'" );
  // will add an alternative
  assertNoThrow( w->addLemmaAnnotation( args ) );
  // will not throw, because an new alternative is created
  args["class"] = "name";
  args["generate_id"] = "blaat";
  assertNoThrow( w->addLemmaAnnotation( args ) );
  // lemma in different set
  vector<LemmaAnnotation*> vec;
  LemmaAnnotation *l = w->getLemmaAnnotations( "", vec ); // return all lemma's
  assertTrue( l != 0 );
  assertEqual( vec.size(), 2 );
  assertEqual( w->xmlstring(), "<w xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.2.w.11\"><t>naam</t><pos class=\"N(soort,ev,basis,zijd,stan)\" set=\"https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn\"/><lemma class=\"naam\"/><alt xml:id=\"WR-P-E-J-0000000001.p.1.s.2.w.11.alt-pos.1\" auth=\"no\"><pos annotator=\"testscript\" class=\"N\" set=\"https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn\"/></alt><alt xml:id=\"WR-P-E-J-0000000001.p.1.s.2.w.11.alt-pos.2\" auth=\"no\"><pos annotator=\"testscript\" class=\"N\" set=\"https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn\"/></alt><alt xml:id=\"WR-P-E-J-0000000001.p.1.s.2.w.11.alt-lem.1\" auth=\"no\"><lemma annotator=\"testscript\" class=\"naam\"/></alt><alt xml:id=\"WR-P-E-J-0000000001.p.1.s.2.w.11.blaat.1\" auth=\"no\"><lemma annotator=\"testscript\" class=\"name\"/></alt></w>" );

  assertNoThrow( editDoc.declare( AnnotationType::LEMMA,
				  "andere-set") );
  args["set"] = "andere-set";
  assertNoThrow( w->addLemmaAnnotation( args ) );
  args["class"] = "naam";
  assertNoThrow( w->addLemmaAnnotation( args ) ); // add an alternative
  l = w->getLemmaAnnotations( "andere-set", vec ); // get all lemma's in andere-set
  assertTrue( l != 0 );
  assertEqual( vec.size(), 1 );
  l = w->getLemmaAnnotations( "lemmas-nl", vec );// get all lemma's in lemmas-nl (the former default set)
  assertTrue( l != 0 );
  assertEqual( vec.size(), 2 );
  l = w->getLemmaAnnotations( "", vec ); // get ALL lemma's and alternatives
  assertTrue( l != 0 );
  assertEqual( vec.size(), 3 );
}

void edit_test004b( ){
  startTestSerie( " Add a token default-set annotation which gives a name clash " );
  Document editDoc( "file='tests/example.xml'" );
  // grab a word which already has morhological info)
  FoliaElement *w = 0;
  assertNoThrow( w = editDoc["WR-P-E-J-0000000001.p.1.s.3.w.5"] );
  // add morhological information without specifying a set (should take default
  // set), but this will clash with existing tag!
  FoliaElement *l = new MorphologyLayer( &editDoc );
  FoliaElement *m = new Morpheme( &editDoc );
  l->append( m );
  FoliaElement *t = new TextContent( getArgs("value='hand', offset='0'"),
				     &editDoc );
  m->append( t );
  m = new Morpheme( &editDoc );
  l->append( m );
  t = new TextContent( getArgs("value='schrift', offset='4'"), &editDoc );
  m->append( t );
  m = new Morpheme( &editDoc );
  l->append( m );
  t = new TextContent( getArgs("value='en', offset='11'"), &editDoc );
  m->append( t );
  assertThrow( w->append( l ), DuplicateAnnotationError );
}

void edit_test004c( ){
  startTestSerie( " Add an Entity in another set. giving a name clash " );
  Document editDoc( "file='tests/example.xml'" );
  // grab all entity layers
  vector<EntitiesLayer *> elv = editDoc.doc()->select<EntitiesLayer>();
  assertEqual( elv.size(), 6 );
  assertEqual( elv[1]->sett(), "mwu-set" );
  KWargs args;
  args["set"] = "mwu-set";
  args["class"] = "OKE";
  Entity *e = new Entity( args, &editDoc );
  assertNoThrow( elv[1]->append( e ) );
  args["set"] = "http://raw.github.com/proycon/folia/master/setdefinitions/namedentities.foliaset.xml";
  args["class"] = "WRONG";
  e = new Entity( args, &editDoc );
  assertThrow( elv[1]->append( e ), DuplicateAnnotationError );
  assertEqual( elv[1]->xmlstring(), "<entities xmlns=\"http://ilk.uvt.nl/folia\"><entity class=\"politicalparty\" set=\"mwu-set\"><wref id=\"WR-P-E-J-0000000001.sandbox.2.s.1.w.1.m.1\" t=\"CDA\"/></entity><entity class=\"OKE\" set=\"mwu-set\"/></entities>" );
}

void edit_test005a( ){
  startTestSerie( " Adding an alternative token annotation " );
  Document doc( "file='tests/example.xml'" );
  FoliaElement *w = doc["WR-P-E-J-0000000001.p.1.s.2.w.11"];
  KWargs args;
  args["class"]="V";
  args["set"]="https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn";
  assertNoThrow( w->addPosAnnotation( args ) );
  vector<Alternative*> alt = w->alternatives(); // all alternatives
  string sett = doc.defaultset(AnnotationType::POS);
  vector<Alternative*> alt2 = w->alternatives(sett);
  assertTrue( alt.size() == 1 );
  assertTrue( alt2.size() == 1 );
  assertTrue( alt[0] == alt2[0] );
  FoliaElement *p = 0;
  assertTrue( p = w->annotation<PosAnnotation>( sett ) );
  assertTrue( p->isinstance<PosAnnotation>() );

  std::vector<Alternative *> alt3;
  assertNoThrow( alt3 = w->alternatives(PosAnnotation_t, sett) );
  assertTrue( alt3.size() == 1 );
  assertTrue( alt[0] == alt3[0] );

  assertEqual( w->xmlstring(), "<w xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.2.w.11\"><t>naam</t><pos class=\"N(soort,ev,basis,zijd,stan)\" set=\"https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn\"/><lemma class=\"naam\"/><alt xml:id=\"WR-P-E-J-0000000001.p.1.s.2.w.11.alt-pos.1\" auth=\"no\"><pos class=\"V\" set=\"https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn\"/></alt></w>" );

  std::vector<PosAnnotation*> pv;
  PosAnnotation *pos1 = w->getPosAnnotations( sett, pv );
  assertTrue( pos1 != 0 );
  assertEqual( pv.size(), 1 );
  assertEqual( pos1->xmlstring(), "<pos xmlns=\"http://ilk.uvt.nl/folia\" class=\"N(soort,ev,basis,zijd,stan)\" set=\"https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn\"/>" );
  assertEqual( pv[0]->xmlstring(), "<pos xmlns=\"http://ilk.uvt.nl/folia\" class=\"V\" set=\"https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn\"/>" );


}

void edit_test005b( ){
  startTestSerie( " Adding a morpology layer in a different set" );
  FoliaElement *w = 0;
  Document editDoc( "file='tests/example.xml'" );
  assertNoThrow( w = editDoc["WR-P-E-J-0000000001.p.1.s.3.w.5"] );
  // add morhological information without specifying a set (should take default
  // set),

  MorphologyLayer *l = 0;
  KWargs args;
  assertNoThrow( l = w->addMorphologyLayer(args) );
  FoliaElement *m = new Morpheme( &editDoc );
  l->append( m );
  m->settext( "hand", 0 );
  m = new Morpheme( &editDoc );
  l->append( m );
  m->settext( "schrift", 4 );
  m = new Morpheme( &editDoc );
  l->append( m );
  m->settext( "en", 11 );
}

void edit_test005c( ){
  startTestSerie( " Adding TextContents in different classes" );
  FoliaElement *s = 0;
  Document editDoc( "file='tests/example.xml'" );
  assertNoThrow( s = editDoc["WR-P-E-J-0000000001.p.1.s.1"] );
  assertNoThrow( s->settext( "text1", "test1" ) );
  assertNoThrow( s->settext( "text1", "test2" ) );
}

void edit_test005d( ){
  startTestSerie( " Adding String tags in several classes" );
  FoliaElement *s = 0;
  Document editDoc( "file='tests/example.xml'" );
  assertNoThrow( editDoc.declare( AnnotationType::STRING,
				  "stringtypes") );
  assertNoThrow( s = editDoc["WR-P-E-J-0000000001.p.1.s.1"] );
  FoliaElement *t = new String( getArgs("class='test1'"), &editDoc );
  assertNoThrow( t->settext( "text1", "test1" ) );
  assertNoThrow( s->append( t ) );
  t = new String( getArgs("class='test2'"), &editDoc );
  assertNoThrow( s->append( t ) );
  assertNoThrow( t->settext( "text2" ) );
  assertNoThrow( t->settext( "text2" , "test2" ) );
  assertTrue( t->xmlstring() == "<str xmlns=\"http://ilk.uvt.nl/folia\" class=\"test2\"><t>text2</t><t class=\"test2\">text2</t></str>" );
  t = new String( getArgs("class='test3'"), &editDoc );
  assertNoThrow( s->append( t ) );
  assertNoThrow( t->settext( "V&D bestaat niet meer" ) );
  assertEqual( t->xmlstring(), "<str xmlns=\"http://ilk.uvt.nl/folia\" class=\"test3\"><t>V&amp;D bestaat niet meer</t></str>" );
  assertEqual( t->text(), "V&D bestaat niet meer" );
}

void edit_test006( ){
  startTestSerie( " Correcting text " );
  FoliaElement *w = 0;
  Document doc( "file='tests/example.xml'" );
  assertNoThrow( w = doc["WR-P-E-J-0000000001.p.1.s.8.w.11"] ); // stippelijn
  assertNoThrow( w->correct("new='stippellijn', set='corrections', class='spelling',annotator='testscript', annotatortype='auto'"  ) );
  FoliaElement *c = 0;
  assertNoThrow( c = w->annotation<Correction>() );
  assertTrue( c->getOriginal()->index(0)->text() == "stippelijn" );
  assertTrue( c->getNew()->index(0)->text() == "stippellijn" );
  assertTrue( w->text() == "stippellijn" );

  assertEqual( w->xmlstring(),"<w xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.11\"><correction xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.11.correction.1\" annotator=\"testscript\" annotatortype=\"auto\" class=\"spelling\"><new><t>stippellijn</t></new><original auth=\"no\"><t>stippelijn</t></original></correction><pos class=\"FOUTN(soort,ev,basis,zijd,stan)\" set=\"https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn\"/><lemma class=\"stippelijn\"/></w>" );

}

void edit_test007( ){
  startTestSerie( " Correcting Token Annotation " );
  Document doc( "file='tests/example.xml'" );
  FoliaElement *w = 0;
  assertNoThrow( w = doc["WR-P-E-J-0000000001.p.1.s.8.w.11"] ); // alweer stippelijn
  FoliaElement *oldpos = w->annotation<PosAnnotation>();
  KWargs args;
  args["class"]="N(soort,ev,basis,zijd,stan)";
  args["set"]="https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn";
  FoliaElement *newpos
    = new PosAnnotation( args, &doc );
  args = getArgs( "set='corrections', class='spelling', annotator='testscript', annotatortype='auto'" );
  assertNoThrow( w->correct( oldpos, newpos, args ) );
  FoliaElement *c = 0;
  assertTrue( (c = w->annotation<Correction>()) != 0 );
  assertTrue( c->getOriginal()->index(0) == oldpos );
  assertTrue( (*c->getNew())[0] == newpos );

  assertEqual( w->xmlstring(), "<w xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.11\"><t>stippelijn</t><correction xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.11.correction.1\" annotator=\"testscript\" annotatortype=\"auto\" class=\"spelling\"><new><pos class=\"N(soort,ev,basis,zijd,stan)\" set=\"https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn\"/></new><original auth=\"no\"><pos class=\"FOUTN(soort,ev,basis,zijd,stan)\" set=\"https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn\"/></original></correction><lemma class=\"stippelijn\"/></w>" );
}

void edit_test008( ){
  startTestSerie( " Suggesting a text correction " );
  Document editDoc( "file='tests/example.xml'" );
  FoliaElement *w = editDoc["WR-P-E-J-0000000001.p.1.s.8.w.11"]; // stippelijn
  assertNoThrow( w->correct( "suggestion='stippellijn', set='corrections', class='spelling',annotator='testscript', annotatortype='auto'" ) );
  FoliaElement *c = 0;
  assertNoThrow( c = w->annotation<Correction>() );
  assertNoThrow( c = c->suggestions()[0] );
  assertTrue( c->text() == "stippellijn" );
  assertEqual( w->xmlstring(), "<w xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.11\"><t>stippelijn</t><pos class=\"FOUTN(soort,ev,basis,zijd,stan)\" set=\"https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn\"/><lemma class=\"stippelijn\"/><correction xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.11.correction.1\" annotator=\"testscript\" annotatortype=\"auto\" class=\"spelling\"><suggestion auth=\"no\"><t>stippellijn</t></suggestion></correction></w>" );
}

void edit_test009a( ){
  startTestSerie( " Exception on duplicate ID " );
  Document editDoc( "file='tests/example.xml'" );
  FoliaElement *w = 0;
  assertNoThrow( w = editDoc["WR-P-E-J-0000000001.p.1.s.8.w.11"] );
  KWargs kw;
  kw["text"] = "stippellijn";
  kw[XML_ID] = "WR-P-E-J-0000000001.p.1.s.8.w.11";
  assertThrow( w->sentence()->addWord( kw ),
	       DuplicateIDError );
}

// void edit_test009b( ){
//   startTestSerie( " Exception on adding TextContent of wrong level " );
//   Document editDoc( "file='tests/example.xml'" );
//   FoliaElement *w = 0;
//   assertNoThrow( w = editDoc["WR-P-E-J-0000000001.p.1.s.8.w.11"] );
//   assertThrow( w->settext( "bla", "original" ), ValueError );
// }

void edit_test009c( ){
  startTestSerie( " Exception on adding duplicate TextContent " );
   Document editDoc( "file='tests/example.xml'" );
   FoliaElement *w = 0;
   assertNoThrow( w = editDoc["WR-P-E-J-0000000001.p.1.s.8.w.11"] );
   KWargs args;
   args["value"] = "blah";
   TextContent *t = new TextContent( args );
   assertThrow( w->append( t ), DuplicateAnnotationError );
}

void edit_test010( ){
  startTestSerie( " Creating an initially document-less tokenannotation element and adding it to a word " );
  Document editDoc( "file='tests/example.xml'" );
  FoliaElement *w = 0;
  assertNoThrow( w = editDoc["WR-P-E-J-0000000001.p.1.s.8.w.11"] );
  FoliaElement *pos = 0;
  assertThrow( pos
	       = new PosAnnotation( getArgs("set='fakecgn', class='N'") ),
	       ValueError );
  assertNoThrow( editDoc.declare( AnnotationType::POS,
				  "fakecgn") );
  assertNoThrow( pos = new PosAnnotation( getArgs("set='fakecgn', class='N'" ),
					   &editDoc ) );
  assertNoThrow( w->append( pos ) );
  assertTrue( pos == w->annotation<PosAnnotation>("fakecgn") );
  assertTrue( pos->parent() == w );
  assertTrue( pos->doc() == w->doc() );

  assertTrue( w->xmlstring() == "<w xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.11\"><t>stippelijn</t><pos class=\"FOUTN(soort,ev,basis,zijd,stan)\" set=\"https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn\"/><lemma class=\"stippelijn\"/><pos class=\"N\" set=\"fakecgn\"/></w>" );
}

void edit_test011(){
  startTestSerie( " Adding Subtoken annotation (morphological analysis)" );
  Document editDoc( "file='tests/example.xml'" );
  FoliaElement *w = editDoc["WR-P-E-J-0000000001.p.1.s.5.w.3"];
  MorphologyLayer *l = new MorphologyLayer(0);
  w->append( l );
  Morpheme *m = new Morpheme( &editDoc );
  l->append( m );
  TextContent *t = new TextContent( getArgs("value='handschrift', offset='0'"),
				    &editDoc );
  m->append( t );
  Feature *f = new Feature( getArgs("subset='type', class='stem'"),
			    &editDoc );
  m->append( f );
  f = new Feature( getArgs("subset='function', class='lexical'"),
		   &editDoc );
  m->append( f );
  m = new Morpheme( &editDoc );
  l->append( m );
  t = new TextContent( getArgs("value='en', offset='11'"), &editDoc );
  m->append( t );
  f = new Feature( getArgs( "subset='type', class='suffix'" ), &editDoc );
  m->append( f );
  f = new Feature( getArgs("subset='function', class='plural'"), &editDoc );
  m->append( f );
  assertTrue( len(l) ==  2 );  // 2 morphemes
  assertTrue( isinstance( l->index(0), Morpheme_t ) );
  assertTrue( l->index(0)->text() == "handschrift" );
  assertTrue( l->index(0)->feat("type") == "stem" );
  assertTrue( l->index(0)->feat("function") == "lexical" );
  assertTrue( l->index(1)->text() == "en" );
  assertTrue( l->index(1)->feat("type") == "suffix" );
  assertTrue( l->index(1)->feat("function") == "plural" );
  assertTrue( w->xmlstring() == "<w xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.5.w.3\"><t>handschriften</t><pos class=\"N(soort,mv,basis)\" set=\"https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn\"/><lemma class=\"handschrift\"/><morphology><morpheme><t offset=\"0\">handschrift</t><feat class=\"stem\" subset=\"type\"/><feat class=\"lexical\" subset=\"function\"/></morpheme><morpheme><t offset=\"11\">en</t><feat class=\"suffix\" subset=\"type\"/><feat class=\"plural\" subset=\"function\"/></morpheme></morphology></w>" );
}

void edit_test012(){
  startTestSerie( " Edit Check - Adding Alignment" );
  Document editDoc( "file='tests/example.xml'" );
  FoliaElement *w = editDoc["WR-P-E-J-0000000001.p.1.s.6.w.8"];
  Alignment *a = new Alignment( getArgs("class='coreference'"), &editDoc );
  w->append( a );
  AlignReference *ar = new AlignReference( getArgs("id='WR-P-E-J-0000000001.p.1.s.6.w.1', type='w', t='appel'") );
  a->append(ar);
  // missing type or some random type is no longer an error
  assertNoThrow( ar = new AlignReference( getArgs("id='wrong', type='word'") ) );
  assertNoThrow( ar = new AlignReference( getArgs("id='wrong'") ));
  ar = new AlignReference( getArgs("id='WR-P-E-J-0000000001.p.1.s.6.w.2', type='w'") );
  a->append(ar);
  assertEqual( a->resolve()[0], editDoc["WR-P-E-J-0000000001.p.1.s.6.w.1"] );
  assertEqual( a->resolve()[1], editDoc["WR-P-E-J-0000000001.p.1.s.6.w.2"] );
  string res = w->xmlstring();
#if FOLIA_INT_VERSION > 116
  assertEqual( res, "<w xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.6.w.8\"><t>ze</t><pos class=\"VNW(pers,pron,stan,red,3,mv)\" set=\"https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn\"/><lemma class=\"ze\"/><relation class=\"coreference\"><xref id=\"WR-P-E-J-0000000001.p.1.s.6.w.1\" t=\"appel\" type=\"w\"/><xref id=\"WR-P-E-J-0000000001.p.1.s.6.w.2\" type=\"w\"/></relation></w>" );
#else
  assertEqual( res, "<w xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.6.w.8\"><t>ze</t><pos class=\"VNW(pers,pron,stan,red,3,mv)\" set=\"https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn\"/><lemma class=\"ze\"/><alignment class=\"coreference\"><aref id=\"WR-P-E-J-0000000001.p.1.s.6.w.1\" t=\"appel\" type=\"w\"/><aref id=\"WR-P-E-J-0000000001.p.1.s.6.w.2\" type=\"w\"/></alignment></w>" );
#endif
}

void edit_test013(){
  startTestSerie( " Adding Span Annotation (syntax) " );
  Document editDoc( "file='tests/example.xml'" );
  FoliaElement *s = editDoc["WR-P-E-J-0000000001.p.1.s.4"];
  //sentence: 'De hoofdletter A wordt gebruikt voor het originele handschrift .'
  FoliaElement *layer = s->append( new SyntaxLayer(&editDoc) );
  FoliaElement *sent = layer->append( new SyntacticUnit( getArgs("class='s'" ),
							 &editDoc ));
  FoliaElement *np = sent->append( new SyntacticUnit( getArgs("class='np'"),
						      &editDoc ) );
  FoliaElement *su = np->append( new SyntacticUnit( getArgs("class='det'" ),
						     &editDoc ) );
  su->append( editDoc["WR-P-E-J-0000000001.p.1.s.4.w.1"] );
  su = np->append( new SyntacticUnit( getArgs("class='n'"), &editDoc ) );
  su->append( editDoc["WR-P-E-J-0000000001.p.1.s.4.w.2"] );
  su = np->append( new SyntacticUnit( getArgs("class='n'"), &editDoc ) );
  su->append( editDoc["WR-P-E-J-0000000001.p.1.s.4.w.3"] );
  FoliaElement *vp = sent->append( new SyntacticUnit( getArgs("class='vp'"),
						       &editDoc ) );
  FoliaElement *vps = vp->append( new SyntacticUnit( getArgs("class='vp'" ),
						     &editDoc ) );
  su = vps->append( new SyntacticUnit( getArgs("class='v'"), &editDoc ) );
  su->append( editDoc["WR-P-E-J-0000000001.p.1.s.4.w.4"] );
  su = vps->append( new SyntacticUnit( getArgs("class='participle'"),
				       &editDoc ) );
  su->append( editDoc["WR-P-E-J-0000000001.p.1.s.4.w.5"] );
  FoliaElement *pp = vp->append( new SyntacticUnit( getArgs("class='pp'"),
						    &editDoc ) );
  su = pp->append( new SyntacticUnit( getArgs("class='prep'"), &editDoc ) );
  su->append( editDoc["WR-P-E-J-0000000001.p.1.s.4.w.6"] );
  FoliaElement *nps = pp->append( new SyntacticUnit( getArgs("class='np'"),
						      &editDoc ) );
  su = nps->append( new SyntacticUnit( getArgs("class='det'"), &editDoc ) );
  su->append( editDoc["WR-P-E-J-0000000001.p.1.s.4.w.7"] );
  su = nps->append( new SyntacticUnit( getArgs("class='adj'"), &editDoc ) );
  su->append( editDoc["WR-P-E-J-0000000001.p.1.s.4.w.8"] );
  su = nps->append( new SyntacticUnit( getArgs("class='n'"), &editDoc ) );
  su->append( editDoc["WR-P-E-J-0000000001.p.1.s.4.w.9"] );

  assertTrue( layer->xmlstring() == "<syntax xmlns=\"http://ilk.uvt.nl/folia\"><su class=\"s\"><su class=\"np\"><su class=\"det\"><wref id=\"WR-P-E-J-0000000001.p.1.s.4.w.1\" t=\"De\"/></su><su class=\"n\"><wref id=\"WR-P-E-J-0000000001.p.1.s.4.w.2\" t=\"hoofdletter\"/></su><su class=\"n\"><wref id=\"WR-P-E-J-0000000001.p.1.s.4.w.3\" t=\"A\"/></su></su><su class=\"vp\"><su class=\"vp\"><su class=\"v\"><wref id=\"WR-P-E-J-0000000001.p.1.s.4.w.4\" t=\"wordt\"/></su><su class=\"participle\"><wref id=\"WR-P-E-J-0000000001.p.1.s.4.w.5\" t=\"gebruikt\"/></su></su><su class=\"pp\"><su class=\"prep\"><wref id=\"WR-P-E-J-0000000001.p.1.s.4.w.6\" t=\"voor\"/></su><su class=\"np\"><su class=\"det\"><wref id=\"WR-P-E-J-0000000001.p.1.s.4.w.7\" t=\"het\"/></su><su class=\"adj\"><wref id=\"WR-P-E-J-0000000001.p.1.s.4.w.8\" t=\"originele\"/></su><su class=\"n\"><wref id=\"WR-P-E-J-0000000001.p.1.s.4.w.9\" t=\"handschrift\"/></su></su></su></su></su></syntax>" );
}

void edit_test013b() {
  startTestSerie( " Correction of a Span Annotation " );
  Document editDoc( "file='tests/example.xml'" );
  FoliaElement *cell = 0;
  assertNoThrow( cell = editDoc["example.cell"] );
  FoliaElement *el = 0;
  assertNoThrow( el = cell->annotation<EntitiesLayer>() );
  FoliaElement *old = editDoc["example.radboud.university.nijmegen.org"];
  vector<FoliaElement*> wrefs = editDoc["example.radboud.university.nijmegen.org"]->wrefs();
  FoliaElement *newEnt = new Entity( getArgs("class='loc', set='http://raw.github.com/proycon/folia/master/setdefinitions/namedentities.foliaset.xml'"),
				     &editDoc );
  for ( size_t i=0; i < wrefs.size(); ++i ){
    newEnt->append( wrefs[i] );
  }
  KWargs args = getArgs( "set='corrections',class='wrongclass'" );
  assertNoThrow( el->correct(old,newEnt,args) );
  assertEqual( el->xmlstring(), "<entities xmlns=\"http://ilk.uvt.nl/folia\"><correction xml:id=\"example.cell.correction.1\" class=\"wrongclass\"><new><entity class=\"loc\" set=\"http://raw.github.com/proycon/folia/master/setdefinitions/namedentities.foliaset.xml\"><wref id=\"example.table.1.w.6\" t=\"Radboud\"/><wref id=\"example.table.1.w.7\" t=\"University\"/><wref id=\"example.table.1.w.8\" t=\"Nijmegen\"/></entity></new><original auth=\"no\"><entity xml:id=\"example.radboud.university.nijmegen.org\" class=\"org\" set=\"http://raw.github.com/proycon/folia/master/setdefinitions/namedentities.foliaset.xml\"><wref id=\"example.table.1.w.6\" t=\"Radboud\"/><wref id=\"example.table.1.w.7\" t=\"University\"/><wref id=\"example.table.1.w.8\" t=\"Nijmegen\"/><comment annotator=\"proycon\">This is our university!</comment></entity></original></correction></entities>" );
}

void edit_test013c() {
  startTestSerie( " default set for SpanAnnotation " );
  Document editDoc( "file='tests/example.xml'" );
  FoliaElement *sent = 0;
  assertNoThrow( sent = editDoc["WR-P-E-J-0000000001.p.1.s.1"] ); // first sentence
  FoliaElement *el = 0;
  assertNoThrow( el = new EntitiesLayer( &editDoc ) );
  assertNoThrow( sent->append( el ) );
  FoliaElement *word = editDoc["WR-P-E-J-0000000001.head.1.s.1.w.1"];
  FoliaElement *newEnt = new Entity( getArgs("class='org', set='http://raw.github.com/proycon/folia/master/setdefinitions/namedentities.foliaset.xml'" ),
				      &editDoc );
  assertNoThrow( newEnt->append( word ) );
  assertNoThrow( el->append( newEnt ) );
  assertEqual( el->xmlstring(), "<entities xmlns=\"http://ilk.uvt.nl/folia\"><entity class=\"org\" set=\"http://raw.github.com/proycon/folia/master/setdefinitions/namedentities.foliaset.xml\"><wref id=\"WR-P-E-J-0000000001.head.1.s.1.w.1\" t=\"Stemma\"/></entity></entities>" );
}


void edit_test013d() {
  startTestSerie( " default set for SpanAnnotation " );
  Document editDoc( "file='tests/example.xml'" );
  FoliaElement *sent = 0;
  assertNoThrow( sent = editDoc["WR-P-E-J-0000000001.p.1.s.1"] ); // first sentence
  FoliaElement *el = 0;
  assertNoThrow( el = new EntitiesLayer( &editDoc ) );
  assertNoThrow( sent->append( el ) );
  FoliaElement *word1 = editDoc["WR-P-E-J-0000000001.p.1.s.1.w.1"];
  FoliaElement *word2 = editDoc["WR-P-E-J-0000000001.p.1.s.1.w.2"];
  FoliaElement *ent1 = new Entity( getArgs("class='org', set='http://raw.github.com/proycon/folia/master/setdefinitions/namedentities.foliaset.xml'" ), &editDoc );
  assertNoThrow( ent1->append( word1 ) );
  FoliaElement *ent2 = new Entity( getArgs("class='rel', set='http://raw.github.com/proycon/folia/master/setdefinitions/namedentities.foliaset.xml'"),
				    &editDoc );
  assertNoThrow( ent2->append( word2 ) );
  KWargs args = getArgs( "set='corrections',class='wrongclass'" );
  Correction *corr = 0;
  assertNoThrow( corr = sent->correct(ent1,ent2,args) );
  assertNoThrow( el->append( corr ) );
  assertEqual( el->xmlstring(), "<entities xmlns=\"http://ilk.uvt.nl/folia\"><correction xml:id=\"WR-P-E-J-0000000001.p.1.s.1.correction.1\" class=\"wrongclass\"><new><entity class=\"rel\" set=\"http://raw.github.com/proycon/folia/master/setdefinitions/namedentities.foliaset.xml\"><wref id=\"WR-P-E-J-0000000001.p.1.s.1.w.2\" t=\"is\"/></entity></new><original auth=\"no\"><entity class=\"org\" set=\"http://raw.github.com/proycon/folia/master/setdefinitions/namedentities.foliaset.xml\"><wref id=\"WR-P-E-J-0000000001.p.1.s.1.w.1\" t=\"Stemma\"/></entity></original></correction></entities>" );
}

void edit_test014() {
  startTestSerie( " Replacing an annotation " );
  Document editDoc( "file='tests/example.xml'" );
  FoliaElement *word = editDoc["WR-P-E-J-0000000001.p.1.s.3.w.14"];
  word->replace( new PosAnnotation( getArgs("class='BOGUS', set='https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn'"),  &editDoc ) );
  assertEqual( len(word->annotations<PosAnnotation>() ) , 1 );
  assertEqual( word->annotation<PosAnnotation>()->cls(), "BOGUS" );
  assertEqual( word->xmlstring(),"<w xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.3.w.14\"><t>plaats</t><lemma class=\"plaats\"/><pos class=\"BOGUS\" set=\"https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn\"/></w>" );
}

void edit_test015(){
  startTestSerie( " Removing an annotation " );
  Document editDoc( "file='tests/example.xml'" );
  FoliaElement *word = editDoc["WR-P-E-J-0000000001.p.1.s.3.w.14"];
  word->remove( word->annotation<PosAnnotation>() );
  assertThrow( word->annotations<PosAnnotation>(), NoSuchAnnotation );
  assertTrue( word->xmlstring() == "<w xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.3.w.14\"><t>plaats</t><lemma class=\"plaats\"/></w>" );
}

void edit_test016(){
  startTestSerie( " Time Stamp " );
  Document editDoc( "file='tests/example.xml'" );
  FoliaElement *word = editDoc["WR-P-E-J-0000000001.p.1.s.8.w.16"];
  FoliaElement *pos = word->annotation<PosAnnotation>();
  assertNoThrow( pos->setDateTime( "1982-12-15T19:00:01" ) );

  assertEqual( pos->xmlstring(), "<pos xmlns=\"http://ilk.uvt.nl/folia\" class=\"WW(pv,tgw,met-t)\" datetime=\"1982-12-15T19:00:01\" set=\"https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn\"/>" );
}

void edit_test017a(){
  startTestSerie( " Altering word text (pre v1.5)" );
  Document editDoc( "file='tests/examplev1.4.xml'" );
  // Important note: directly altering text is usually bad practise, you'll want to use proper corrections instead.
  FoliaElement *word = editDoc["WR-P-E-J-0000000001.p.1.s.8.w.9"];
  assertTrue( word->text() == "terweil" );
  assertNoThrow( word->settext("terwijl") );
  assertEqual( word->xmlstring(), "<w xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.9\"><t>terwijl</t><errordetection class=\"spelling\"/><pos class=\"VG(onder)\" set=\"https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn\"/><lemma class=\"terweil\"/></w>" );
}

void edit_test017b(){
  startTestSerie( " Altering word text (after v1.5)" );
  Document editDoc( "file='tests/examplev1.5.xml'" );
  // Important note: directly altering text is usually bad practise, you'll want to use proper corrections instead.
  FoliaElement *word = editDoc["example.p.1.s.1.w.8"];
  assertTrue( word->text() == "oeuvre" );
  string s;
  assertNoThrow( s = word->xmlstring() ); // a valid doc
  assertThrow( word->settext("œuvre"), InconsistentText ); // invalid attempt
  assertNoThrow( s = word->xmlstring() ); // make sure the doc is unhurted
}

void edit_test018a(){
  startTestSerie( " Altering sentence text (untokenized by definition)" );
  Document editDoc( "file='tests/example.xml'" );
  FoliaElement *s = editDoc["WR-P-E-J-0000000001.p.1.s.1"];
  assertTrue( !s->hastext() ); // No text directly associated
  // but text() can be retrieved from children.
  assertTrue( s->text() == "Stemma is een ander woord voor stamboom." );
  // now set this text on the sentence too
  assertNoThrow( s->settext( "Stemma is een ander woord voor stamboom." ) );
  assertTrue( s->hastext() ); // Now there IS direct text
}

void edit_test018b(){
  startTestSerie( " Altering sentence text (pre v1.5) (untokenized by definition)" );
  Document editDoc( "file='tests/examplev1.4.xml'" );
  FoliaElement *s = editDoc["WR-P-E-J-0000000001.p.1.s.8"];
  // 1 get text() dynamic from children
  assertEqual( s->text(), "Een volle lijn duidt op een verwantschap , terweil een stippelijn op een onzekere verwantschap duidt ." );
  // 2 set the text correct here
  assertNoThrow( s->settext("Een volle lijn duidt op een verwantschap, terwijl een stippellijn op een onzekere verwantschap duidt." ) );
  // 3 set the incorrect here too in another class
  assertNoThrow( s->settext("Een volle lijn duidt op een verwantschap, terweil een stippelijn op een onzekere verwantschap duidt.", "original" ) );
  // 5 get the text from the children again
  assertEqual( s->text(), "Een volle lijn duidt op een verwantschap , terweil een stippelijn op een onzekere verwantschap duidt ." );
  // check the text in the 'original' class
  assertTrue( s->hastext("original") );
  assertEqual( s->text("original"), "Een volle lijn duidt op een verwantschap, terweil een stippelijn op een onzekere verwantschap duidt." );

  assertEqual( s->xmlstring(), "<s xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.8\"><t>Een volle lijn duidt op een verwantschap, terwijl een stippellijn op een onzekere verwantschap duidt.</t><t class=\"original\">Een volle lijn duidt op een verwantschap, terweil een stippelijn op een onzekere verwantschap duidt.</t><w xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.1\"><t>Een</t><pos class=\"LID(onbep,stan,agr)\" set=\"https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn\"/><lemma class=\"een\"/></w><quote xml:id=\"WR-P-E-J-0000000001.p.1.s.8.q.1\"><w xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.2\"><t>volle</t><pos class=\"ADJ(prenom,basis,met-e,stan)\" set=\"https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn\"/><lemma class=\"vol\"/></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.3\"><t>lijn</t><pos class=\"N(soort,ev,basis,zijd,stan)\" set=\"https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn\"/><lemma class=\"lijn\"/></w></quote><w xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.4\"><t>duidt</t><pos class=\"WW(pv,tgw,met-t)\" set=\"https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn\"/><lemma class=\"duiden\"/></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.5\"><t>op</t><pos class=\"VZ(init)\" set=\"https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn\"/><lemma class=\"op\"/></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.6\"><t>een</t><pos class=\"LID(onbep,stan,agr)\" set=\"https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn\"/><lemma class=\"een\"/></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.7\"><t>verwantschap</t><pos class=\"N(soort,ev,basis,zijd,stan)\" set=\"https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn\"/><lemma class=\"verwantschap\"/></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.8\"><t>,</t><pos class=\"LET()\" set=\"https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn\"/><lemma class=\",\"/></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.9\"><t>terweil</t><errordetection class=\"spelling\"/><pos class=\"VG(onder)\" set=\"https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn\"/><lemma class=\"terweil\"/></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.10\"><t>een</t><pos class=\"LID(onbep,stan,agr)\" set=\"https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn\"/><lemma class=\"een\"/></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.11\"><t>stippelijn</t><pos class=\"FOUTN(soort,ev,basis,zijd,stan)\" set=\"https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn\"/><lemma class=\"stippelijn\"/></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.12\"><t>op</t><pos class=\"VZ(init)\" set=\"https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn\"/><lemma class=\"op\"/></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.13\"><t>een</t><pos class=\"LID(onbep,stan,agr)\" set=\"https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn\"/><lemma class=\"een\"/></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.14\"><t>onzekere</t><pos class=\"ADJ(prenom,basis,met-e,stan)\" set=\"https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn\"/><lemma class=\"onzeker\"/><correction xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.14.c.1\" class=\"spelling\"><suggestion auth=\"no\" n=\"1/2\"><t>twijfelachtige</t></suggestion><suggestion auth=\"no\" n=\"2/2\"><t>ongewisse</t></suggestion></correction></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.15\"><t>verwantschap</t><pos class=\"N(soort,ev,basis,zijd,stan)\" datetime=\"2011-07-20T19:00:01\" set=\"https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn\"/><lemma class=\"verwantschap\"/></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.16\"><t>duidt</t><pos class=\"WW(pv,tgw,met-t)\" set=\"https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn\"/><lemma class=\"duiden\"/></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.17\"><t>.</t><pos class=\"LET()\" set=\"https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn\"/><lemma class=\".\"/></w><observations><observation class=\"ei_ij_error\"><wref id=\"WR-P-E-J-0000000001.p.1.s.8.w.9\" t=\"terweil\"/><desc>Confusion between EI and IJ diphtongues</desc></observation></observations></s>" );
}

void edit_test018c(){
  startTestSerie( " Altering sentence text (untokenized by definition)" );
  Document editDoc( "file='tests/examplev1.5.xml'" );
  FoliaElement *s = editDoc["example.p.1.s.1"];
  // 1 get text() dynamic from children
  assertEqual( s->text(), "Is het creëren van een volwaardig literair oeuvre voorbehouden aan schrijvers als Couperus, Haasse, of Grunberg?" );
  // 2 get the text of the Sentence itself (which contains spaces and tabs!
  assertEqual( s->stricttext(), "Is het creëren van een volwaardig literair oeuvre voorbehouden aan schrijvers\n"
"	als Couperus, 	Haasse, of\n"
"	Grunberg?" );
  // 3 try to change the text
  assertThrow( s->settext("This MUST fail!" ), InconsistentText );
  // 4 but we may add differnet text in another class
  assertNoThrow( s->settext("And now for something completely different.", "larch" ) );
  // 5 get the text from the children
  // 5a check the text in the 'original' class
  assertEqual( s->text("original"), "een volwaardig œuvre" );
  // 5b check the text in the 'larch' class
  assertTrue( s->hastext("larch") );
  assertEqual( s->text("larch"),"And now for something completely different." );
  // 6 get one word
  FoliaElement *w = editDoc["example.p.1.s.1.w.8"];
  // check it's REAL text, including spaced
  assertEqual( w->text("original"), " œuvre  " );
  // check the normalized value
  assertEqual( normalize_spaces(w->text("original")), "œuvre" );
  // We may NOT changed te text
  assertThrow( w->settext("oeuvre","original"), InconsistentText );
  // But we MAY change the formatting
  assertNoThrow( w->settext("œuvre", "original") );
  assertEqual( w->text("original"), "œuvre" );
}

void edit_test019(){
  startTestSerie( " Error Detection " );
  Document editDoc( "file='tests/example.xml'" );
  FoliaElement *word = editDoc["WR-P-E-J-0000000001.p.1.s.8.w.11"]; // stippelijn
  word->append( new ErrorDetection( getArgs("class='spelling', annotator='testscript', annotatortype='auto'"),  &editDoc ) );
  assertEqual( word->annotation<ErrorDetection>()->cls(), "spelling" );
}

void text_test03(){
  startTestSerie( " Invalid Text (Misspelled word) " );
  string xml= "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
"<FoLiA xmlns=\"http://ilk.uvt.nl/folia\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xml:id=\"test\" version=\"1.5\">"
"  <metadata type=\"native\">"
"    <annotations>"
"      <token-annotation annotator=\"ucto\" annotatortype=\"auto\" datetime=\"2017-09-25T10:29:52\" set=\"tokconfig-nld\"/>"
"    </annotations>"
"  </metadata>"
"  <text xml:id=\"example.text\">"
"    <p xml:id=\"example.p.1\">"
"      <t>Is het creëren van een volwaardig literair oeuvre voorbehouden aan schrijvers als Couperus, Haasse, of Grunberg?</t>"
"      <s xml:id=\"example.p.1.s.1\">"
"        <t>Is het creëren van een volwaardig literrair oeuvre voorbehouden aan schrijvers"
"	als Couperus, 	Haasse, of"
"	Grunberg?</t>"
"      </s>"
"    </p>"
"  </text>"
"</FoLiA>";
  Document doc;
  assertThrow( doc.readFromString(xml), InconsistentText );
}


void text_test04(){
  startTestSerie( " Invalid Text (Missing word) " );
  string xml= "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
"<FoLiA xmlns=\"http://ilk.uvt.nl/folia\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xml:id=\"test\" version=\"1.5\">"
"  <metadata type=\"native\">"
"    <annotations>"
"      <token-annotation annotator=\"ucto\" annotatortype=\"auto\" datetime=\"2017-09-25T10:29:52\" set=\"tokconfig-nld\"/>"
"    </annotations>"
"  </metadata>"
"  <text xml:id=\"example.text\">"
"    <p xml:id=\"example.p.1\">"
"      <t>Is het creëren van een volwaardig literair oeuvre voorbehouden aan schrijvers als Couperus, Haasse, of Grunberg?</t>"
"      <s xml:id=\"example.p.1.s.1\">"
"        <t>Is het creëren van een volwaardig oeuvre voorbehouden aan schrijvers"
"	als Couperus, 	Haasse, of"
"	Grunberg?</t>"
"      </s>"
"    </p>"
"  </text>"
"</FoLiA>";
  Document doc;
  assertThrow( doc.readFromString(xml), InconsistentText );
}


void text_test05(){
  startTestSerie( " Valid Text (Intermittent Redundancy) " );
  string xml= "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
"<FoLiA xmlns=\"http://ilk.uvt.nl/folia\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xml:id=\"test\" version=\"1.5\">"
"  <metadata type=\"native\">"
"    <annotations>"
"      <token-annotation annotator=\"ucto\" annotatortype=\"auto\" datetime=\"2017-09-25T10:29:52\" set=\"tokconfig-nld\"/>"
"    </annotations>"
"  </metadata>"
"  <text xml:id=\"example.text\">"
"      <t>Is het creëren van een volwaardig literair oeuvre voorbehouden aan schrijvers als Couperus, Haasse, of Grunberg? Of kan ik het ook?</t>"
"    <p xml:id=\"example.p.1\">"
"      <!-- Note: no text here on paragraph level -->"
"      <s xml:id=\"example.p.1.s.1\">"
"        <t>Is het creëren van een volwaardig oeuvre voorbehouden aan schrijvers"
"	als Couperus, 	Haasse, of"
"	Grunberg?</t>"
"      </s>"
"      <s xml:id=\"example.p.1.s.2\">"
"        <t>Of kan ik"
"het    ook   ?"
"            </t>"
"      </s>"
"    </p>"
"  </text>"
"</FoLiA>";
  Document doc;
  assertThrow( doc.readFromString(xml), InconsistentText );
}

void text_test06(){
  startTestSerie( " Vallid Text (Multiple classes) " );
  string xml= "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
"<FoLiA xmlns=\"http://ilk.uvt.nl/folia\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xml:id=\"test\" version=\"1.5\">"
"  <metadata type=\"native\">"
"    <annotations>"
"      <token-annotation annotator=\"ucto\" annotatortype=\"auto\" datetime=\"2017-09-25T10:29:52\" set=\"tokconfig-nld\"/>"
"    </annotations>"
"  </metadata>"
"  <text xml:id=\"example.text\">"
"    <p xml:id=\"example.p.1\">"
"      <t>Is het creëren van een volwaardig literair oeuvre voorbehouden aan schrijvers als Couperus, Haasse, of Grunberg?</t>"
"      <t class=\"missingword\">Is het creëren van een volwaardig oeuvre voorbehouden aan schrijvers als Couperus, Haasse, of Grunberg?</t>"
"      <s xml:id=\"example.p.1.s.1\">"
"        <t>Is het creëren van een volwaardig literrair oeuvre voorbehouden aan schrijvers"
"	als Couperus, 	Haasse, of"
"	Grunberg?</t>"
"        <t class=\"missingword\">Is het creëren van een volwaardig oeuvre voorbehouden aan schrijvers"
"	als Couperus, 	Haasse, of"
"	Grunberg?</t>"
"      </s>"
"    </p>"
"  </text>"
"</FoLiA>";
  Document doc;
  assertThrow( doc.readFromString(xml), InconsistentText );
}

void text_test07(){
  startTestSerie( "Validation - No text checking on (nested) morphemes" );
  string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
"<FoLiA xmlns=\"http://ilk.uvt.nl/folia\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xml:id=\"test\" version=\"1.5\">"
"  <metadata type=\"native\">"
"    <annotations>"
"      <token-annotation annotator=\"ucto\" annotatortype=\"auto\" datetime=\"2017-09-25T10:29:52\" set=\"tokconfig-nld\"/>"
"      <pos-annotation set=\"https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn\" annotator=\"frog\" annotatortype=\"auto\" />"
"      <pos-annotation annotator=\"frog-mbma-1.0\" annotatortype=\"auto\" datetime=\"2017-04-20T16:48:45\" set=\"http://ilk.uvt.nl/folia/sets/frog-mbpos-clex\"/>"
"      <lemma-annotation set=\"lemmas-nl\" annotator=\"tadpole\" annotatortype=\"auto\" />"
"      <morphological-annotation annotator=\"proycon\" annotatortype=\"manual\" />"
"    </annotations>"
"  </metadata>"
"  <text xml:id=\"example.text\">"
"      <w xml:id=\"WR-P-E-J-0000000001.p.1.s.2.w.16\">"
"        <t>genealogie</t>"
"        <pos class=\"N(soort,ev,basis,zijd,stan)\" set=\"https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn\"/>"
"        <lemma class=\"genealogie\"/>"
"        <morphology>"
"          <morpheme class=\"complex\">"
"            <t>genealogie</t>"
"            <feat class=\"[[genealogisch]adjective[ie]]noun/singular\" subset=\"structure\"/>"
"            <pos class=\"N\" set=\"http://ilk.uvt.nl/folia/sets/frog-mbpos-clex\"/>"
"            <morpheme class=\"complex\">"
"              <feat class=\"N_A*\" subset=\"applied_rule\"/>"
"              <feat class=\"[[genealogisch]adjective[ie]]noun\" subset=\"structure\"/>"
"              <pos class=\"N\" set=\"http://ilk.uvt.nl/folia/sets/frog-mbpos-clex\"/>"
"              <morpheme class=\"stem\">"
"                <t>genealogisch</t>"
"                <pos class=\"A\" set=\"http://ilk.uvt.nl/folia/sets/frog-mbpos-clex\"/>"
"              </morpheme>"
"              <morpheme class=\"affix\">"
"                <t>ie</t>"
"                <feat class=\"[ie]\" subset=\"structure\"/>"
"              </morpheme>"
"             </morpheme>"
"             <morpheme class=\"inflection\">"
"              <feat class=\"singular\" subset=\"inflection\"/>"
"             </morpheme>"
"          </morpheme>"
"        </morphology>"
"      </w>"
"  </text>"
"</FoLiA>";
  Document doc;
  assertNoThrow( doc.readFromString(xml) );
}

void text_test08(){
  startTestSerie( "Text - Offset validation" );
  string xml= "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
"<FoLiA xmlns=\"http://ilk.uvt.nl/folia\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xml:id=\"test\" version=\"1.5\">"
"  <metadata type=\"native\">"
"    <annotations>"
"      <token-annotation annotator=\"ucto\" annotatortype=\"auto\" datetime=\"2017-09-25T10:29:52\" set=\"tokconfig-nld\"/>"
"    </annotations>"
"  </metadata>"
"  <text xml:id=\"example.text\">"
"    <p xml:id=\"example.p.1\">"
"      <s xml:id=\"example.p.1.s.1\">"
"        <t>Is het creëren van een volwaardig literair oeuvre voorbehouden aan schrijvers"
"	 als Couperus, 	Haasse, of"
"	 Grunberg?</t>"
"        <w xml:id=\"example.p.1.s.1.w.1\" class=\"WORD\">"
"          <t offset=\"0\">Is</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.2\" class=\"WORD\">"
"          <t offset=\"3\">het</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.3\" class=\"WORD\">"
"          <t offset=\"7\">creëren</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.4\" class=\"WORD\">"
"          <t offset=\"15\">van</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.5\" class=\"WORD\">"
"          <t offset=\"19\">een</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.6\" class=\"WORD\">"
"          <t offset=\"23\">volwaardig</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.7\" class=\"WORD\">"
"          <t offset=\"34\">literair</t>"
"         </w>"
"       <w xml:id=\"example.p.1.s.1.w.8\" class=\"WORD\">"
"         <t offset=\"43\">oeuvre</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.9\" class=\"WORD\">"
"         <t offset=\"50\">voorbehouden</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.10\" class=\"WORD\">"
"         <t offset=\"63\">aan</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.11\" class=\"WORD\">"
"         <t offset=\"67\">schrijvers</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.12\" class=\"WORD\">"
"         <t offset=\"79\">als</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.13\" class=\"WORD\" space=\"no\">"
"         <t offset=\"83\">Couperus</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.14\" class=\"PUNCTUATION\">"
"         <t offset=\"91\">,</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.15\" class=\"WORD\" space=\"no\">"
"         <t offset=\"94\">Haasse</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.16\" class=\"PUNCTUATION\">"
"         <t offset=\"100\">,</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.17\" class=\"WORD\">"
"         <t offset=\"102\">of</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.18\" class=\"WORD\" space=\"no\">"
"         <t offset=\"106\">Grunberg</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.19\" class=\"PUNCTUATION\">"
"         <t offset=\"114\">?</t>"
"        </w>"
"      </s>"
"    </p>"
"  </text>"
"</FoLiA>";
  Document doc;
  assertNoThrow( doc.readFromString(xml) );
}

void text_test09(){
  startTestSerie( "Text - Offset validation (invalid)" );
  string xml= "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
"<FoLiA xmlns=\"http://ilk.uvt.nl/folia\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xml:id=\"test\" version=\"1.5\">"
"  <metadata type=\"native\">"
"    <annotations>"
"      <token-annotation annotator=\"ucto\" annotatortype=\"auto\" datetime=\"2017-09-25T10:29:52\" set=\"tokconfig-nld\"/>"
"    </annotations>"
"  </metadata>"
"  <text xml:id=\"example.text\">"
"    <p xml:id=\"example.p.1\">"
"      <s xml:id=\"example.p.1.s.1\">"
"        <t>Is het creëren van een volwaardig literair oeuvre voorbehouden aan schrijvers"
"	 als Couperus, 	Haasse, of"
"	 Grunberg?</t>"
"        <w xml:id=\"example.p.1.s.1.w.1\" class=\"WORD\">"
"          <t offset=\"0\">Is</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.2\" class=\"WORD\">"
"          <t offset=\"3\">het</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.3\" class=\"WORD\">"
"          <t offset=\"7\">creëren</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.4\" class=\"WORD\">"
"          <t offset=\"10\">van</t> <!-- this one is invalid -->"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.5\" class=\"WORD\">"
"          <t offset=\"19\">een</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.6\" class=\"WORD\">"
"          <t offset=\"24\">volwaardig</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.7\" class=\"WORD\">"
"          <t offset=\"34\">literair</t>"
"         </w>"
"       <w xml:id=\"example.p.1.s.1.w.8\" class=\"WORD\">"
"         <t offset=\"43\">oeuvre</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.9\" class=\"WORD\">"
"         <t offset=\"50\">voorbehouden</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.10\" class=\"WORD\">"
"         <t offset=\"63\">aan</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.11\" class=\"WORD\">"
"         <t offset=\"67\">schrijvers</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.12\" class=\"WORD\">"
"         <t offset=\"79\">als</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.13\" class=\"WORD\" space=\"no\">"
"         <t offset=\"83\">Couperus</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.14\" class=\"PUNCTUATION\">"
"         <t offset=\"91\">,</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.15\" class=\"WORD\" space=\"no\">"
"         <t offset=\"94\">Haasse</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.16\" class=\"PUNCTUATION\">"
"         <t offset=\"100\">,</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.17\" class=\"WORD\">"
"         <t offset=\"102\">of</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.18\" class=\"WORD\" space=\"no\">"
"         <t offset=\"106\">Grunberg</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.19\" class=\"PUNCTUATION\">"
"         <t offset=\"114\">?</t>"
"        </w>"
"      </s>"
"    </p>"
"  </text>"
"</FoLiA>";
  Document doc;
  assertThrow( doc.readFromString(xml), UnresolvableTextContent );
}

void text_test10(){
  startTestSerie( "Text - Offset validation with explicit references" );
  string xml= "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
"<FoLiA xmlns=\"http://ilk.uvt.nl/folia\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xml:id=\"test\" version=\"1.5\">"
"  <metadata type=\"native\">"
"    <annotations>"
"      <token-annotation annotator=\"ucto\" annotatortype=\"auto\" datetime=\"2017-09-25T10:29:52\" set=\"tokconfig-nld\"/>"
"    </annotations>"
"  </metadata>"
"  <text xml:id=\"example.text\">"
"    <p xml:id=\"example.p.1\">"
"      <t>Is het creëren van een volwaardig literair oeuvre voorbehouden aan schrijvers"
"	 als Couperus, 	Haasse, of"
"	 Grunberg?</t>"
"      <s xml:id=\"example.p.1.s.1\">"
"        <w xml:id=\"example.p.1.s.1.w.1\" class=\"WORD\">"
"          <t offset=\"0\" ref=\"example.p.1\">Is</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.2\" class=\"WORD\">"
"          <t offset=\"3\" ref=\"example.p.1\">het</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.3\" class=\"WORD\">"
"          <t offset=\"7\" ref=\"example.p.1\">creëren</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.4\" class=\"WORD\">"
"          <t offset=\"15\" ref=\"example.p.1\">van</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.5\" class=\"WORD\">"
"          <t offset=\"19\" ref=\"example.p.1\">een</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.6\" class=\"WORD\">"
"          <t offset=\"23\" ref=\"example.p.1\">volwaardig</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.7\" class=\"WORD\">"
"          <t offset=\"34\" ref=\"example.p.1\">literair</t>"
"         </w>"
"       <w xml:id=\"example.p.1.s.1.w.8\" class=\"WORD\">"
"         <t offset=\"43\" ref=\"example.p.1\">oeuvre</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.9\" class=\"WORD\">"
"         <t offset=\"50\" ref=\"example.p.1\">voorbehouden</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.10\" class=\"WORD\">"
"         <t offset=\"63\" ref=\"example.p.1\">aan</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.11\" class=\"WORD\">"
"         <t offset=\"67\" ref=\"example.p.1\">schrijvers</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.12\" class=\"WORD\">"
"         <t offset=\"79\" ref=\"example.p.1\">als</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.13\" class=\"WORD\" space=\"no\">"
"         <t offset=\"83\" ref=\"example.p.1\">Couperus</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.14\" class=\"PUNCTUATION\">"
"         <t offset=\"91\" ref=\"example.p.1\">,</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.15\" class=\"WORD\" space=\"no\">"
"         <t offset=\"94\" ref=\"example.p.1\">Haasse</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.16\" class=\"PUNCTUATION\">"
"         <t offset=\"100\" ref=\"example.p.1\">,</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.17\" class=\"WORD\">"
"         <t offset=\"102\" ref=\"example.p.1\">of</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.18\" class=\"WORD\" space=\"no\">"
"         <t offset=\"106\" ref=\"example.p.1\">Grunberg</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.19\" class=\"PUNCTUATION\">"
"         <t offset=\"114\" ref=\"example.p.1\">?</t>"
"        </w>"
"      </s>"
"    </p>"
"  </text>"
"</FoLiA>";
  Document doc;
  assertNoThrow( doc.readFromString(xml) );
  assertEqual( doc["example.p.1.s.1.w.19"]->textcontent()->getreference(),
	       doc["example.p.1"] ); // testing resolving explicit reference
}

void text_test11(){
  startTestSerie( "Text - Offset validation with text markup (with text modifiers like br" );
  string xml= "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
"<FoLiA xmlns=\"http://ilk.uvt.nl/folia\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xml:id=\"test\" version=\"1.5\">"
"  <metadata type=\"native\">"
"    <annotations>"
"      <token-annotation annotator=\"ucto\" annotatortype=\"auto\" datetime=\"2017-09-25T10:29:52\" set=\"tokconfig-nld\"/>"
"      <style-annotation />"
"    </annotations>"
"  </metadata>"
"  <text xml:id=\"example.text\">"
"    <p xml:id=\"example.p.1\">"
"      <s xml:id=\"example.p.1.s.1\">"
"        <t>Is het creëren van een <t-style class=\"emphasis\">volwaardig</t-style> literair oeuvre voorbehouden aan schrijvers<br/>\tals <t-str xlink:href=\"https://nl.wikipedia.org/wiki/Louis_Couperus\" xlink:type=\"simple\">Couperus</t-str>, 	Haasse, of \tGrunberg?</t>"
"        <w xml:id=\"example.p.1.s.1.w.1\" class=\"WORD\">"
"          <t offset=\"0\">Is</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.2\" class=\"WORD\">"
"          <t offset=\"3\">het</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.3\" class=\"WORD\">"
"          <t offset=\"7\">creëren</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.4\" class=\"WORD\">"
"          <t offset=\"15\">van</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.5\" class=\"WORD\">"
"          <t offset=\"19\">een</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.6\" class=\"WORD\">"
"          <t offset=\"23\">volwaardig</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.7\" class=\"WORD\">"
"          <t offset=\"34\">literair</t>"
"         </w>"
"       <w xml:id=\"example.p.1.s.1.w.8\" class=\"WORD\">"
"         <t offset=\"43\">oeuvre</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.9\" class=\"WORD\">"
"         <t offset=\"50\">voorbehouden</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.10\" class=\"WORD\">"
"         <t offset=\"63\">aan</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.11\" class=\"WORD\">"
"         <t offset=\"67\">schrijvers</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.12\" class=\"WORD\">"
"         <t offset=\"79\">als</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.13\" class=\"WORD\" space=\"no\">"
"         <t offset=\"83\">Couperus</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.14\" class=\"PUNCTUATION\">"
"         <t offset=\"91\">,</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.15\" class=\"WORD\" space=\"no\">"
"         <t offset=\"94\">Haasse</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.16\" class=\"PUNCTUATION\">"
"         <t offset=\"100\">,</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.17\" class=\"WORD\">"
"         <t offset=\"102\">of</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.18\" class=\"WORD\" space=\"no\">"
"         <t offset=\"106\">Grunberg</t>"
"       </w>"
"       <w xml:id=\"example.p.1.s.1.w.19\" class=\"PUNCTUATION\">"
"         <t offset=\"114\">?</t>"
"        </w>"
"      </s>"
"    </p>"
"  </text>"
"</FoLiA>";
  Document doc;
  assertNoThrow( doc.readFromString(xml) );
  assertEqual( doc["example.p.1.s.1.w.19"]->textcontent()->getreference(),
	       doc["example.p.1.s.1"] ); // testing resolving implicit reference
}

void text_test12(){
  startTestSerie( "Validation - Text Validation on String" );
  string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
"<FoLiA xmlns=\"http://ilk.uvt.nl/folia\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xml:id=\"test\" version=\"1.5\" generator=\"hand\">"
"  <metadata type=\"native\">"
"    <annotations>"
"      <token-annotation annotator=\"ucto\" annotatortype=\"auto\" datetime=\"2017-09-25T10:29:52\" set=\"tokconfig-nld\"/>"
"      <style-annotation />"
"    </annotations>"
"  </metadata>"
"  <text xml:id=\"example.text\">"
"    <p xml:id=\"example.p.1\">"
"      <s xml:id=\"example.p.1.s.1\">"
"        <t>Is het creëren van een <t-style class=\"emphasis\">volwaardig</t-style> literair oeuvre voorbehouden aan schrijvers<br/>\tals <t-str xlink:href=\"https://nl.wikipedia.org/wiki/Louis_Couperus\" xlink:type=\"simple\">Couperus</t-str>, 	Haasse, of"
"\tGrunberg?</t>"
"        <str xml:id=\"example.string\">"
"            <t offset=\"7\">creëren</t>"
"        </str>"
"      </s>"
"    </p>"
"  </text>"
"</FoLiA>";
  Document doc;
  assertNoThrow( doc.readFromString(xml) );
  assertEqual( doc["example.string"]->textcontent()->getreference(),
	       doc["example.p.1.s.1"] ); // testing resolving implicit reference
}

void text_test13a(){
  startTestSerie( "Validation - Text Validation on Correction (single text layer)" );
  string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
"<FoLiA xmlns=\"http://ilk.uvt.nl/folia\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xml:id=\"test\" version=\"1.5\" generator=\"hand\">"
"  <metadata type=\"native\">"
"    <annotations>"
"      <token-annotation annotator=\"ucto\" annotatortype=\"auto\" datetime=\"2017-09-25T10:29:52\" set=\"tokconfig-nld\"/>"
"      <style-annotation />"
"    </annotations>"
"  </metadata>"
"  <text xml:id=\"example.text\">"
"    <p xml:id=\"example.p.1\">"
"      <s xml:id=\"example.p.1.s.1\">"
"        <t>Is het creëren van een volwaardig literair oeuvre voorbehouden aan schrijvers?</t>"
"        <w xml:id=\"example.p.1.s.1.w.1\" class=\"WORD\">"
"          <t offset=\"0\">Is</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.2\" class=\"WORD\">"
"          <t offset=\"3\">het</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.3\" class=\"WORD\">"
"          <correction>"
"           <new>"
"              <t offset=\"7\">creëren</t>"
"           </new>"
"           <original auth=\"no\">"
"              <t offset=\"7\">creeren</t>"
"           </original>"
"          </correction>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.4\" class=\"WORD\">"
"          <t offset=\"15\">van</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.5\" class=\"WORD\">"
"          <t offset=\"19\">een</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.6\" class=\"WORD\">"
"          <t offset=\"23\">volwaardig</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.7\" class=\"WORD\">"
"          <t offset=\"34\">literair</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.8\" class=\"WORD\">"
"          <t offset=\"43\">oeuvre</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.9\" class=\"WORD\">"
"          <t offset=\"50\">voorbehouden</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.10\" class=\"WORD\">"
"          <t offset=\"63\">aan</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.11\" class=\"WORD\" space=\"no\">"
"          <t offset=\"67\">schrijvers</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.12\" class=\"WORD\">"
"          <t offset=\"77\">?</t>"
"        </w>"
"      </s>"
"    </p>"
"  </text>"
 "</FoLiA>";
  Document doc;
  assertNoThrow( doc.readFromString(xml) );
}

void text_test13b(){
  startTestSerie( "Validation - Text Validation on Correction (Double text layers)" );
  string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
"<FoLiA xmlns=\"http://ilk.uvt.nl/folia\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xml:id=\"test\" version=\"1.5\" generator=\"hand\">"
"  <metadata type=\"native\">"
"    <annotations>"
"      <token-annotation annotator=\"ucto\" annotatortype=\"auto\" datetime=\"2017-09-25T10:29:52\" set=\"tokconfig-nld\"/>"
"      <style-annotation />"
"    </annotations>"
"  </metadata>"
"  <text xml:id=\"example.text\">"
"    <p xml:id=\"example.p.1\">"
"      <s xml:id=\"example.p.1.s.1\">"
"        <t>Is het creëren van een volwaardig literair oeuvre voorbehouden aan schrijvers?</t>"
"        <t class=\"original\">Is het creeren van een volwaardig litterair oeuvre voorbehouden aan schrijvers?</t>"
"        <w xml:id=\"example.p.1.s.1.w.1\" class=\"WORD\">"
"          <t offset=\"0\">Is</t>"
"          <t class=\"original\" offset=\"0\">Is</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.2\" class=\"WORD\">"
"          <t offset=\"3\">het</t>"
"          <t class=\"original\" offset=\"3\">het</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.3\" class=\"WORD\">"
"          <correction>"
"           <new>"
"              <t offset=\"7\">creëren</t>"
"           </new>"
"           <original auth=\"no\">"
"              <t class=\"original\" offset=\"7\">creeren</t>"
"           </original>"
"          </correction>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.4\" class=\"WORD\">"
"          <t offset=\"15\">van</t>"
"          <t class=\"original\" offset=\"15\">van</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.5\" class=\"WORD\">"
"          <t offset=\"19\">een</t>"
"          <t class=\"original\" offset=\"19\">een</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.6\" class=\"WORD\">"
"          <t offset=\"23\">volwaardig</t>"
"          <t class=\"original\" offset=\"23\">volwaardig</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.7\" class=\"WORD\">"
"          <t offset=\"34\">literair</t>"
"          <t class=\"original\" offset=\"34\">litterair</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.8\" class=\"WORD\">"
"          <t offset=\"43\">oeuvre</t>"
"          <t class=\"original\" offset=\"44\">oeuvre</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.9\" class=\"WORD\">"
"          <t offset=\"50\">voorbehouden</t>"
"          <t class=\"original\" offset=\"51\">voorbehouden</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.10\" class=\"WORD\">"
"          <t offset=\"63\">aan</t>"
"          <t class=\"original\" offset=\"64\">aan</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.11\" class=\"WORD\" space=\"no\">"
"          <t offset=\"67\">schrijvers</t>"
"          <t class=\"original\" offset=\"68\">schrijvers</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.12\" class=\"WORD\">"
"          <t offset=\"77\">?</t>"
"          <t class=\"original\" offset=\"78\">?</t>"
"        </w>"
"      </s>"
"    </p>"
"  </text>"
  "</FoLiA>";
  Document doc;
  assertNoThrow( doc.readFromString( xml ) );
}

void text_test13c(){
  startTestSerie( "Validation - Text Validation on Correction (Double text layers, structural changes)" );
  string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
"<FoLiA xmlns=\"http://ilk.uvt.nl/folia\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xml:id=\"test\" version=\"1.5\" generator=\"diy\">"
"  <metadata type=\"native\">"
"    <annotations>"
"      <token-annotation annotator=\"ucto\" annotatortype=\"auto\" datetime=\"2017-09-25T10:29:52\" set=\"tokconfig-nld\"/>"
"      <style-annotation />"
"    </annotations>"
"  </metadata>"
"  <text xml:id=\"example.text\">"
"    <p xml:id=\"example.p.1\">"
"      <s xml:id=\"example.p.1.s.1\">"
"        <t>Is het creëren van een volwaardig literair oeuvre voorbehouden aan schrijvers?</t>"
"        <t class=\"original\">Is het creeren van een volwaardig litterair oeuvre voor behouden aan schrijvers?</t>"
"        <w xml:id=\"example.p.1.s.1.w.1\" class=\"WORD\">"
"          <t offset=\"0\">Is</t>"
"          <t class=\"original\" offset=\"0\">Is</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.2\" class=\"WORD\">"
"          <t offset=\"3\">het</t>"
"          <t class=\"original\" offset=\"3\">het</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.3\" class=\"WORD\">"
"          <correction>"
"           <new>"
"              <t offset=\"7\">creëren</t>"
"           </new>"
"           <original auth=\"no\">"
"              <t class=\"original\" offset=\"7\">creeren</t>"
"           </original>"
"          </correction>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.4\" class=\"WORD\">"
"          <t offset=\"15\">van</t>"
"          <t class=\"original\" offset=\"15\">van</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.5\" class=\"WORD\">"
"          <t offset=\"19\">een</t>"
"          <t class=\"original\" offset=\"19\">een</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.6\" class=\"WORD\">"
"          <t offset=\"23\">volwaardig</t>"
"          <t class=\"original\" offset=\"23\">volwaardig</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.7\" class=\"WORD\">"
"          <t offset=\"34\">literair</t>"
"          <t class=\"original\" offset=\"34\">litterair</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.8\" class=\"WORD\">"
"          <t offset=\"43\">oeuvre</t>"
"          <t class=\"original\" offset=\"44\">oeuvre</t>"
"        </w>"
"        <correction>"
"         <new>"
"            <w xml:id=\"example.p.1.s.1.w.9\" class=\"WORD\">"
"              <t offset=\"50\">voorbehouden</t>"
"            </w>"
"         </new>"
"         <original>"
"            <w xml:id=\"example.p.1.s.1.w.9a\" class=\"WORD\">"
"              <t class=\"original\" offset=\"51\">voor</t>"
"            </w>"
"            <w xml:id=\"example.p.1.s.1.w.9b\" class=\"WORD\">"
"              <t class=\"original\" offset=\"56\">behouden</t>"
"            </w>"
"         </original>"
"        </correction>"
"        <w xml:id=\"example.p.1.s.1.w.10\" class=\"WORD\">"
"          <t offset=\"63\">aan</t>"
"          <t class=\"original\" offset=\"65\">aan</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.11\" class=\"WORD\" space=\"no\">"
"          <t offset=\"67\">schrijvers</t>"
"          <t class=\"original\" offset=\"69\">schrijvers</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.12\" class=\"WORD\">"
"          <t offset=\"77\">?</t>"
"          <t class=\"original\" offset=\"79\">?</t>"
"        </w>"
"      </s>"
"    </p>"
"  </text>"
"</FoLiA>";
  Document doc;
  assertNoThrow( doc.readFromString( xml ) );
}

void text_test13d(){
  startTestSerie( "Validation - Text Validation on Correction (Double text layers, structural changes, custom class)" );
  string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
"<FoLiA xmlns=\"http://ilk.uvt.nl/folia\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xml:id=\"test\" version=\"1.5\" generator=\"diy\">"
"  <metadata type=\"native\">"
"    <annotations>"
"      <token-annotation annotator=\"ucto\" annotatortype=\"auto\" datetime=\"2017-09-25T10:29:52\" set=\"tokconfig-nld\"/>"
"      <style-annotation />"
"    </annotations>"
"  </metadata>"
"  <text xml:id=\"example.text\">"
"    <p xml:id=\"example.p.1\">"
"      <s xml:id=\"example.p.1.s.1\">"
"        <t>Is het creëren van een volwaardig literair oeuvre voorbehouden aan schrijvers?</t>"
"        <t class=\"old\">Is het creeren van een volwaardig litterair oeuvre voor behouden aan schrijvers?</t>"
"        <w xml:id=\"example.p.1.s.1.w.1\" class=\"WORD\">"
"          <t offset=\"0\">Is</t>"
"          <t class=\"old\" offset=\"0\">Is</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.2\" class=\"WORD\">"
"          <t offset=\"3\">het</t>"
"          <t class=\"old\" offset=\"3\">het</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.3\" class=\"WORD\">"
"          <correction>"
"           <new>"
"              <t offset=\"7\">creëren</t>"
"           </new>"
"           <original auth=\"no\">"
"              <t class=\"old\" offset=\"7\">creeren</t>"
"           </original>"
"          </correction>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.4\" class=\"WORD\">"
"          <t offset=\"15\">van</t>"
"          <t class=\"old\" offset=\"15\">van</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.5\" class=\"WORD\">"
"          <t offset=\"19\">een</t>"
"          <t class=\"old\" offset=\"19\">een</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.6\" class=\"WORD\">"
"          <t offset=\"23\">volwaardig</t>"
"          <t class=\"old\" offset=\"23\">volwaardig</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.7\" class=\"WORD\">"
"          <t offset=\"34\">literair</t>"
"          <t class=\"old\" offset=\"34\">litterair</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.8\" class=\"WORD\">"
"          <t offset=\"43\">oeuvre</t>"
"          <t class=\"old\" offset=\"44\">oeuvre</t>"
"        </w>"
"        <correction>"
"         <new>"
"            <w xml:id=\"example.p.1.s.1.w.9\" class=\"WORD\">"
"              <t offset=\"50\">voorbehouden</t>"
"            </w>"
"         </new>"
"         <original>"
"            <w xml:id=\"example.p.1.s.1.w.9a\" class=\"WORD\">"
"              <t class=\"old\" offset=\"51\">voor</t>"
"            </w>"
"            <w xml:id=\"example.p.1.s.1.w.9b\" class=\"WORD\">"
"              <t class=\"old\" offset=\"56\">behouden</t>"
"            </w>"
"         </original>"
"        </correction>"
"        <w xml:id=\"example.p.1.s.1.w.10\" class=\"WORD\">"
"          <t offset=\"63\">aan</t>"
"          <t class=\"old\" offset=\"65\">aan</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.11\" class=\"WORD\" space=\"no\">"
"          <t offset=\"67\">schrijvers</t>"
"          <t class=\"old\" offset=\"69\">schrijvers</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.12\" class=\"WORD\">"
"          <t offset=\"77\">?</t>"
"          <t class=\"old\" offset=\"79\">?</t>"
"        </w>"
"      </s>"
"    </p>"
"  </text>"
"</FoLiA>";
  Document doc;
  assertNoThrow( doc.readFromString( xml ) );
}

void text_test13e(){
  startTestSerie( "Validation - Text Validation on complex nested Correction (Double text layers, structural changes, custom class)" );
  // NOTE: Current library implementation won't be able to validate nested layers and will just skip those!
  string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
"<FoLiA xmlns=\"http://ilk.uvt.nl/folia\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xml:id=\"test\" version=\"1.5\" generator=\"diy\">"
"  <metadata type=\"native\">"
"    <annotations>"
"      <token-annotation annotator=\"ucto\" annotatortype=\"auto\" datetime=\"2017-09-25T10:29:52\" set=\"tokconfig-nld\"/>"
"      <style-annotation />"
"    </annotations>"
"  </metadata>"
"  <text xml:id=\"example.text\">"
"    <p xml:id=\"example.p.1\">"
"      <s xml:id=\"example.p.1.s.1\">"
"        <t>Is het creëren van een volwaardig literair oeuvre voorbehouden aan schrijvers?</t>"
"        <t class=\"old\">Is het creeren van een volwaardig litterair oeuvre voor behouden aan schrijvers?</t>"
"        <t class=\"older\">Is het CREEREN van een volwaardig litterair oeuvre voor behouden aan schrijvers?</t>"
"        <w xml:id=\"example.p.1.s.1.w.1\" class=\"WORD\">"
"          <t offset=\"0\">Is</t>"
"          <t class=\"old\" offset=\"0\">Is</t>"
"          <t class=\"older\" offset=\"0\">Is</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.2\" class=\"WORD\">"
"          <t offset=\"3\">het</t>"
"          <t class=\"old\" offset=\"3\">het</t>"
"          <t class=\"older\" offset=\"3\">het</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.3\" class=\"WORD\">"
"          <correction>"
"           <new>"
"              <t offset=\"7\">creëren</t>"
"           </new>"
"           <original auth=\"no\">"
"             <correction>"
"               <new>"
"                 <t class=\"old\" offset=\"7\">creeren</t>"
"               </new>"
"               <original auth=\"no\">"
"                 <t class=\"older\" offset=\"7\">CREEREN</t>"
"               </original>"
"             </correction>"
"           </original>"
"          </correction>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.4\" class=\"WORD\">"
"          <t offset=\"15\">van</t>"
"          <t class=\"old\" offset=\"15\">van</t>"
"          <t class=\"older\" offset=\"15\">van</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.5\" class=\"WORD\">"
"          <t offset=\"19\">een</t>"
"          <t class=\"old\" offset=\"19\">een</t>"
"          <t class=\"older\" offset=\"19\">een</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.6\" class=\"WORD\">"
"          <t offset=\"23\">volwaardig</t>"
"          <t class=\"old\" offset=\"23\">volwaardig</t>"
"          <t class=\"older\" offset=\"23\">volwaardig</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.7\" class=\"WORD\">"
"          <t offset=\"34\">literair</t>"
"          <t class=\"old\" offset=\"34\">litterair</t>"
"          <t class=\"older\" offset=\"34\">litterair</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.8\" class=\"WORD\">"
"          <t offset=\"43\">oeuvre</t>"
"          <t class=\"old\" offset=\"44\">oeuvre</t>"
"          <t class=\"older\" offset=\"44\">oeuvre</t>"
"        </w>"
"        <correction>"
"          <new>"
"            <w xml:id=\"example.p.1.s.1.w.9\" class=\"WORD\">"
"              <t offset=\"50\">voorbehouden</t>"
"            </w>"
"          </new>"
"          <original>"
"            <correction>"
"              <new>"
"                <w xml:id=\"example.p.1.s.1.w.9a\" class=\"WORD\">"
"                  <t class=\"old\" offset=\"51\">voor</t>"
"                </w>"
"                <w xml:id=\"example.p.1.s.1.w.9b\" class=\"WORD\">"
"                  <t class=\"old\" offset=\"56\">behouden</t>"
"                </w>"
"              </new>"
"              <original>"
"                <w xml:id=\"example.p.1.s.1.w.9c\" class=\"WORD\">"
"                  <t class=\"older\" offset=\"51\">voor</t>"
"                </w>"
"                <w xml:id=\"example.p.1.s.1.w.9d\" class=\"WORD\">"
"                  <t class=\"older\" offset=\"56\">behouden</t>"
"                </w>"
"              </original>"
"            </correction>"
"          </original>"
"        </correction>"
"        <w xml:id=\"example.p.1.s.1.w.10\" class=\"WORD\">"
"          <t offset=\"63\">aan</t>"
"          <t class=\"old\" offset=\"65\">aan</t>"
"          <t class=\"older\" offset=\"65\">aan</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.11\" class=\"WORD\" space=\"no\">"
"          <t offset=\"67\">schrijvers</t>"
"          <t class=\"old\" offset=\"69\">schrijvers</t>"
"          <t class=\"older\" offset=\"69\">schrijvers</t>"
"        </w>"
"        <w xml:id=\"example.p.1.s.1.w.12\" class=\"WORD\">"
"          <t offset=\"77\">?</t>"
"          <t class=\"old\" offset=\"79\">?</t>"
"          <t class=\"older\" offset=\"79\">?</t>"
"        </w>"
"      </s>"
"    </p>"
"  </text>"
"</FoLiA>";
  Document doc;
  assertNoThrow( doc.readFromString( xml ) );
}

void text_test13f(){
  startTestSerie( "Validation - Text Validation with redundancy on construction" );
  Document doc( XML_ID"='example', version='1.5'" );
  KWargs args;
  args[XML_ID] = doc.id() + ".text.1";
  Text *text = new Text(args,&doc);
#if FOLIA_INT_VERSION < 115
  doc.append( text );
#else
  doc.setRoot( text );
#endif

  args[XML_ID] = doc.id() + ".s.1";
  Sentence *s = new Sentence( args, &doc );
  s->settext( "De site staat online . " ); //Spaces here!
  text->append( s );
  args[XML_ID] = doc.id() + ".s.1.w.1";
  args["text"] =  "De";
  Word *w = new Word( args, &doc );
  s->append( w );
  args[XML_ID] = doc.id() + ".s.1.w.2";
  args["text"] =  "site";
  w = new Word( args, &doc );
  s->append( w );
  args[XML_ID] = doc.id() + ".s.1.w.3";
  args["text"] =  "staat";
  w = new Word( args, &doc );
  s->append( w );
  args[XML_ID] = doc.id() + ".s.1.w.4";
  args["text"] =  "online";
  w = new Word( args, &doc );
  s->append( w );
  args[XML_ID] = doc.id() + ".s.1.w.5";
  args["text"] =  "."; // No spaces here!
  w = new Word( args, &doc );
  s->append( w );

  doc.setmode("strip");
  assertEqual( doc.getmode(), "mode=strip,checktext," );
  assertEqual( doc.xmlstring(),
	       "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
"<FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\" xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example\" generator=\"\" version=\"\"><metadata type=\"native\"><annotations/></metadata><text xml:id=\"example.text.1\"><s xml:id=\"example.s.1\"><t>De site staat online . </t><w xml:id=\"example.s.1.w.1\"><t>De</t></w><w xml:id=\"example.s.1.w.2\"><t>site</t></w><w xml:id=\"example.s.1.w.3\"><t>staat</t></w><w xml:id=\"example.s.1.w.4\"><t>online</t></w><w xml:id=\"example.s.1.w.5\"><t>.</t></w></s></text></FoLiA>\n" );
}

void text_test14(){
  startTestSerie( "Validation - Text Validation with sentence text delimiter inheritance" );
  Document doc;
  assertNoThrow( doc.readFromFile( "tests/textproblem.xml" ) );
}

void text_test15(){
  startTestSerie( "Validation - Whitespace in text content sanity check" );
  string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
"<FoLiA xmlns=\"http://ilk.uvt.nl/folia\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xml:id=\"test\" version=\"1.5\" >"
"  <metadata type=\"native\">"
"    <annotations>"
"    </annotations>"
"  </metadata>"
"  <text xml:id=\"test.t\">"
"      <s xml:id=\"test.s\"><t>Dit\n"
"         is een rare test.\n"
"         </t>"
"      </s>"
"    </text>"
"</FoLiA>";
  Document doc;
  assertNoThrow( doc.readFromString( xml ) );
  assertEqual( doc["test.s"]->text(), "Dit\n         is een rare test.\n         ");
}

void text_test16(){
  startTestSerie( "Validation - comment in text content sanity check" );
  string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
"<FoLiA xmlns=\"http://ilk.uvt.nl/folia\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xml:id=\"test\" version=\"1.5\" >"
"  <metadata type=\"native\">"
"    <annotations>"
"    </annotations>"
"  </metadata>"
"  <text xml:id=\"test.t\">"
"      <t>This is the real text<comment annotator=\"pkampschreur\" annotatortype=\"manual\" datetime=\"2017-11-01T20:55:50\">Overbodig</comment>.</t>"
"    </text>"
"</FoLiA>";
  Document doc;
  assertNoThrow( doc.readFromString( xml ) );
  UnicodeString txt;
  assertNoThrow( txt = doc["test.t"]->text() );
  assertEqual( txt, "This is the real text.");
}

void text_test17(){
  startTestSerie( "Validation - mixed NFC and NFD encoding" );
  string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
"<FoLiA xmlns=\"http://ilk.uvt.nl/folia\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xml:id=\"test\" version=\"1.5\" >"
"  <text xml:id=\"t1\">"
"   <s xml:id=\"s1\">"
"    <t>ἀντιϰειμένου</t>"
"      <w xml:id=\"w1\">"
"        <t>ἀντιϰειμένου</t>"
"      </w>"
"    </s>"
"  </text>"
"</FoLiA>";
  Document doc;
  assertNoThrow( doc.readFromString( xml ) );
  assertEqual( doc.sentences(0)->text(), "ἀντιϰειμένου");
  assertEqual( doc.words(0)->text(), "ἀντιϰειμένου");
}

void text_test18(){
  startTestSerie( "Validation - paragraph with parts" );
  string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
"<FoLiA xmlns=\"http://ilk.uvt.nl/folia\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xml:id=\"test\" version=\"1.5\" >"
"  <text xml:id=\"t1\">"
"    <p xml:id=\"p1\">"
"      <t>Een test</t>"
"      <part xml:id=\"part1\">"
"        <t>Een</t>"
"      </part>"
"      <part xml:id=\"part2\">"
"        <t> test</t>"
"      </part>"
"    </p>"
"  </text>"
"</FoLiA>";
  Document doc;
  assertNoThrow( doc.readFromString( xml ) );
  //  assertEqual( doc.paragraphs(0)->text(), "Een test");
}

void create_test001( ){
  startTestSerie( " Creating a document from scratch. " );
  Document d( XML_ID"='example'" );
  assertNoThrow( d.declare( AnnotationType::TOKEN,
			    "adhocset",
			    "annotator='proycon'" ) );
  assertEqual( d.defaultset(AnnotationType::TOKEN), "adhocset" );
  assertEqual( d.defaultannotator(AnnotationType::TOKEN), "proycon" );
  FoliaElement *text = 0;
  KWargs kw = getArgs( XML_ID"='" + d.id() + ".text.1'" );
  assertNoThrow( text = d.addText( kw ) );
  kw.clear();
  FoliaElement *s = 0;
  assertNoThrow( s = new Sentence( getArgs("generate_id='" + text->id() + "'"),  &d ) );
  text->append( s );
  kw.clear();
  kw["text"] = "De";
  assertNoThrow( s->addWord( kw ) );
  kw["text"] = "site";
  assertNoThrow( s->addWord( kw ) );
  kw["text"] = "staat";
  assertNoThrow( s->addWord( kw ) );
  kw["text"] = "online";
  assertNoThrow( s->addWord( kw ) );
  kw["text"] = ".";
  assertNoThrow( s->addWord( kw ) );
  assertEqual( d[s->id()]->size(), 5 );
}

void create_test002( ){
  startTestSerie( " Creating a document from scratch. " );
  Document d( XML_ID"='example'" );
  assertNoThrow( d.declare( AnnotationType::POS,
				      "adhocset",
				      "annotator='proycon'" ) );
  assertNoThrow( d.declare( AnnotationType::POS,
				      "myset",
				      "annotator='sloot'" ) );
  assertEqual( d.defaultset(AnnotationType::POS), "" );
  assertEqual( d.defaultannotator(AnnotationType::POS), "" );
  assertEqual( d.defaultannotator(AnnotationType::POS, "myset"), "sloot" );
  string id = d.id() + ".text.1";
  FoliaElement *text = 0;
  KWargs kw = getArgs( XML_ID"='" + id + "'" );
  assertNoThrow( text = d.addText( kw ) );
  kw.clear();
  FoliaElement *s = 0;
  assertNoThrow( s = new Sentence( getArgs("generate_id='" + text->id() + "'"), &d ) );
  text->append( s );
  kw.clear();
  kw["text"] = "landen";
  FoliaElement *w = 0;
  assertNoThrow( w = s->addWord( kw ) );
  kw.clear();
  kw["set"] = "myset";
  kw["class"] = "NP";
  assertNoThrow( w->addAnnotation<PosAnnotation>( kw ) );
  kw["class"] = "VP";
  kw["set"] = "adhocset";
  assertNoThrow( w->addAnnotation<PosAnnotation>( kw ) );
  vector<PosAnnotation*> v = w->select<PosAnnotation>( "adhocset" );
  assertEqual( v.size(), 1 );
  vector<PosAnnotation*> v1 = w->select<PosAnnotation>( "myset" );
  assertEqual( v1.size(), 1 );
  vector<PosAnnotation*> v2 = w->select<PosAnnotation>( "noset" );
  assertEqual( v2.size(), 0 );
  vector<PosAnnotation*> v3 = w->select<PosAnnotation>();
  assertEqual( v3.size(), 2 );
  assertNoThrow( d.save( "/tmp/foliacreatetest002.xml" ) );
  assertEqual( w->xmlstring(), "<w xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example.text.1.s.1.w.1\"><t>landen</t><pos class=\"NP\" set=\"myset\"/><pos class=\"VP\" set=\"adhocset\"/></w>" );
}

void create_test003( ){
  startTestSerie( " Creating a document with gap annotations from scratch. " );
  Document d( XML_ID"='example'" );
  assertNoThrow( d.declare( AnnotationType::GAP,
				      "gap-set",
				      "annotator='sloot'" ) );
  assertNoThrow( d.declare( AnnotationType::GAP,
				      "extended-gap-set",
				      "annotator='sloot'" ) );
  assertEqual( d.defaultset(AnnotationType::GAP), "" );
  assertEqual( d.defaultannotator(AnnotationType::GAP), "" );
  assertEqual( d.defaultannotator(AnnotationType::GAP, "gap-set"), "sloot" );
  assertEqual( d.defaultannotator(AnnotationType::GAP, "extended-gap-set"), "sloot" );
  string id = d.id() + ".text.1";
  FoliaElement *text = 0;
  KWargs kw = getArgs( XML_ID"='" + id + "'" );
  assertNoThrow( text = d.addText( kw ) );
  kw.clear();
  kw["set"] = "gap-set";
  kw["class"] = "NP";
  FoliaElement *g = 0;
  assertNoThrow( g = new Gap( kw, &d ) );
  text->append( g );
  kw.clear();
  kw["set"] = "extended-gap-set";
  kw["class"] = "VP";
  assertNoThrow( g = new Gap( kw, &d ) );
  text->append( g );
  vector<Gap*> v = text->select<Gap>( "gap-set" );
  assertEqual( v.size(), 1 );
  vector<Gap*> v1 = text->select<Gap>( "extended-gap-set" );
  assertEqual( v1.size(), 1 );
  vector<Gap*> v3 = text->select<Gap>();
  assertEqual( v3.size(), 2 );
  assertEqual( text->xmlstring(), "<text xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example.text.1\"><gap class=\"NP\" set=\"gap-set\"/><gap class=\"VP\" set=\"extended-gap-set\"/></text>" );
}

void create_test004( ){
  startTestSerie( " Creating a document from scratch. appending text" );
  Document d( XML_ID"='example'" );
  FoliaElement *text = 0;
  KWargs args;
  args[XML_ID] =  "t.1";
  assertNoThrow( text = d.addText( args ) );
  args[XML_ID] = "p.1";
  FoliaElement *p = 0;
  assertNoThrow( p = new Paragraph( args ) );
  text->append( p );
  args[XML_ID] = "s.1";
  FoliaElement *s = 0;
  assertNoThrow( s = new Sentence( args ) );
  p->append( s );
  args.clear();
  args["value"] = "dit is een tekst";
  args["class"] =  "new";
  TextContent *pT = new TextContent( args  );
  p->append( pT );
  args["value"] = "een andere tekst";
  args["class"] =  "new";
  TextContent *sT = new TextContent( args );
  assertThrow( s->append( sT ), InconsistentText );
}


void create_test005( ){
  startTestSerie( " Creating a document from scratch. appending Words After Sentence" );
  Document d( XML_ID"='example'" );
  FoliaElement *text = 0;
  KWargs args;
  args[XML_ID] =  "t.1";
  assertNoThrow( text = d.addText( args ) );
  args[XML_ID] = "p.1";
  FoliaElement *p = 0;
  assertNoThrow( p = new Paragraph( args ) );
  text->append( p );
  args[XML_ID] = "s.1";
  FoliaElement *s = 0;
  assertNoThrow( s = new Sentence( args ) );
  p->append( s );
  args.clear();
  args["value"] = " dit is \t een \n tekst";
  TextContent *t1 = new TextContent( args  );
  p->append( t1 );
  args["value"] = "dit is een tekst";
  TextContent *t2 = new TextContent( args  );
  // appending a text to the sentence with 'equivalent' text is OK
  assertNoThrow( s->append( t2 ) );
  args["value"] = " dit is een ander tekst";
  TextContent *t3 = new TextContent( args  );
  // appending a text to the sentence with 'different text is NOT OK
  assertThrow( s->append( t3 ), XmlError );
  s->addWord( "dit" );
  s->addWord( "is" );
  assertThrow( s->addWord( "geen" ), XmlError );
  s->addWord( "een" );
  s->addWord( "\t  tekst \n" );
}


void correction_test001a( ){
  startTestSerie( " Split correction " );
  Document corDoc( XML_ID"='example'" );
  assertNoThrow( corDoc.declare( AnnotationType::TOKEN,
				 "adhocset",
				 "annotator='proycon'" ) );
  Text *text = new Text( getArgs(XML_ID"='" + corDoc.id() + ".text.1'") );
  assertNoThrow( corDoc.addText( text ) );
  FoliaElement *s = text->append( new Sentence( getArgs(XML_ID"='" + corDoc.id() + ".s.1'") ) );
  s->append( new Word( getArgs("text='De', " XML_ID"='" + corDoc.id() + ".s.1.w.1'" ),
		       &corDoc ));
  s->append( new Word( getArgs("text='site', " XML_ID"='" + corDoc.id() + ".s.1.w.2'" ),
		       &corDoc ));
  s->append( new Word( getArgs("text='staat', " XML_ID"='" + corDoc.id() + ".s.1.w.3'" ),
		       &corDoc ));
  s->append( new Word( getArgs("text='online', " XML_ID"='" + corDoc.id() + ".s.1.w.4'" ),
		       &corDoc ));
  s->append( new Word( getArgs("text='.', " XML_ID"='" + corDoc.id() + ".s.1.w.5'" ),
		       &corDoc ));
  FoliaElement *w = corDoc.index(corDoc.id() + ".s.1.w.4");
  w->split( new Word( getArgs(XML_ID"='" + corDoc.id() + ".s.1.w.4a', text='on'" ),
		      &corDoc ),
   	    new Word( getArgs(XML_ID"='" + corDoc.id() + ".s.1.w.4b', text='line'" ),
		      &corDoc ));
  //  assertNoThrow( corDoc.save( "/tmp/foliasplit1a.xml" ) );
  s = corDoc.index("example.s.1");
  assertEqual( s->rwords(2)->text(), "on" );
  assertEqual( s->rwords(1)->text(), "line" );
  assertEqual( s->text(), "De site staat on line ." );
  assertEqual( len( s->words() ), 6 );
  assertEqual( s->xmlstring(), "<s xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example.s.1\"><w xml:id=\"example.s.1.w.1\"><t>De</t></w><w xml:id=\"example.s.1.w.2\"><t>site</t></w><w xml:id=\"example.s.1.w.3\"><t>staat</t></w><correction xml:id=\"example.s.1.correction.1\"><new><w xml:id=\"example.s.1.w.4a\"><t>on</t></w><w xml:id=\"example.s.1.w.4b\"><t>line</t></w></new><original auth=\"no\"><w xml:id=\"example.s.1.w.4\"><t>online</t></w></original></correction><w xml:id=\"example.s.1.w.5\"><t>.</t></w></s>" );
}

void correction_test001b( ){
  startTestSerie( " Split suggestion " );
  Document corDoc( XML_ID"='example'" );
  assertNoThrow( corDoc.declare( AnnotationType::TOKEN,
				 "adhocset",
				 "annotator='proycon'" ) );
  Text *text = new Text( getArgs( XML_ID"='" + corDoc.id() + ".text.1'") );
  assertNoThrow( corDoc.addText( text ) );
  FoliaElement *s = text->append( new Sentence( getArgs(XML_ID"='" + corDoc.id() + ".s.1'"  ) ) );
  s->append( new Word( getArgs( "text='De', " XML_ID"='" + corDoc.id() + ".s.1.w.1'" ),
		       &corDoc ) );
  s->append( new Word( getArgs( "text='site', " XML_ID"='" + corDoc.id() + ".s.1.w.2'" ),
		       &corDoc ) );
  s->append( new Word( getArgs("text='staat', " XML_ID"='" + corDoc.id() + ".s.1.w.3'" ),
		       &corDoc ));
  s->append( new Word( getArgs( "text='online', " XML_ID"='" + corDoc.id() + ".s.1.w.4'" ),
		       &corDoc ));
  s->append( new Word( getArgs( "text='.', " XML_ID"='" + corDoc.id() + ".s.1.w.5'" ),
		       &corDoc ));
  FoliaElement *w = corDoc.index(corDoc.id() + ".s.1.w.4");
  Word *w1 = new Word( getArgs("generate_id='" + s->id() + "',text='on'"),
		       &corDoc );
  Word *w2 = new Word( getArgs("generate_id='" + s->id() + "',text='line'" ),
		       &corDoc );
  w->split( w1, w2, "suggest='true'" );
  //  assertNoThrow( corDoc.save( "/tmp/foliasplit1b.xml" ) );
  s = corDoc.index("example.s.1");
  assertEqual( len( s->words() ), 5 );
  assertEqual( s->rwords(1)->text(), "online" );
  assertEqual( s->text(), "De site staat online ." );
  assertEqual( s->xmlstring(), "<s xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example.s.1\"><w xml:id=\"example.s.1.w.1\"><t>De</t></w><w xml:id=\"example.s.1.w.2\"><t>site</t></w><w xml:id=\"example.s.1.w.3\"><t>staat</t></w><correction xml:id=\"example.s.1.correction.1\"><current><w xml:id=\"example.s.1.w.4\"><t>online</t></w></current><suggestion auth=\"no\"><w xml:id=\"example.s.1.w.6\"><t>on</t></w><w xml:id=\"example.s.1.w.7\"><t>line</t></w></suggestion></correction><w xml:id=\"example.s.1.w.5\"><t>.</t></w></s>");
}

void correction_test002(){
  startTestSerie( " Merge corrections " );
  Document corDoc( XML_ID"='example'" );
  assertNoThrow( corDoc.declare( AnnotationType::TOKEN,
				  "adhocset",
				  "annotator='proycon'" ) );
  Text *text = new Text( getArgs(XML_ID"='" + corDoc.id() + ".text.1'") );
  assertNoThrow( corDoc.addText( text ) );
  FoliaElement *s = text->append( new Sentence( getArgs(XML_ID"='" + corDoc.id() + ".s.1'"	) ) );
  s->append( new Word( getArgs( "text='De', " XML_ID"='" + corDoc.id() + ".s.1.w.1'" ),
		       &corDoc ) );
  s->append( new Word( getArgs( "text='site', " XML_ID"='" + corDoc.id() + ".s.1.w.2'" ),
		       &corDoc ) );
  s->append( new Word( getArgs( "text='staat', " XML_ID"='" + corDoc.id() + ".s.1.w.3'" ),
		       &corDoc ) );
  s->append( new Word( getArgs( "text='on', " XML_ID"='" + corDoc.id() + ".s.1.w.4'" ),
		       &corDoc ) );
  s->append( new Word( getArgs( "text='line', " XML_ID"='" + corDoc.id() + ".s.1.w.5'" ),
		       &corDoc ) );
  s->append( new Word( getArgs( "text='.', " XML_ID"='" + corDoc.id() + ".s.1.w.6'" ),
		       &corDoc ) );

  vector<FoliaElement *> ow;
  ow.push_back( corDoc.index(corDoc.id() + ".s.1.w.4") );
  ow.push_back( corDoc.index(corDoc.id() + ".s.1.w.5") );
  s->mergewords( new Word( getArgs(XML_ID"='" + corDoc.id() + ".s.1.w.4-5', text='online'"),
			   &corDoc ),
		 ow );
  //  assertNoThrow( corDoc.save( "/tmp/foliamerge002.xml" ) );
  assertEqual( len(s->words() ),  5 );
  assertEqual( s->text(), "De site staat online ." );
  // incorrection() test, check if newly added word correctly reports being part of a correction
  FoliaElement *w = corDoc.index(corDoc.id() + ".s.1.w.4-5");
  assertTrue( isinstance(w->incorrection(), Correction_t) );
  //incorrection return the correction the word is part of, or None if not part of a correction,
  assertEqual( s->xmlstring(), "<s xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example.s.1\"><w xml:id=\"example.s.1.w.1\"><t>De</t></w><w xml:id=\"example.s.1.w.2\"><t>site</t></w><w xml:id=\"example.s.1.w.3\"><t>staat</t></w><correction xml:id=\"example.s.1.correction.1\"><new><w xml:id=\"example.s.1.w.4-5\"><t>online</t></w></new><original auth=\"no\"><w xml:id=\"example.s.1.w.4\"><t>on</t></w><w xml:id=\"example.s.1.w.5\"><t>line</t></w></original></correction><w xml:id=\"example.s.1.w.6\"><t>.</t></w></s>" );
}

void correction_test003(){
  startTestSerie( " Delete corrections " );
  Document corDoc( XML_ID"='example'" );
  assertNoThrow( corDoc.declare( AnnotationType::TOKEN,
				  "adhocset",
				  "annotator='proycon'" ) );
  Text *text = new Text( getArgs(XML_ID"='" + corDoc.id() + ".text.1'") );
  assertNoThrow( corDoc.addText( text ) );
  FoliaElement *s = text->append( new Sentence( getArgs(XML_ID"='" + corDoc.id() + ".s.1'"	) ) );
  s->append( new Word( getArgs( "text='Ik', " XML_ID"='" + corDoc.id() + ".s.1.w.1'"),
		       &corDoc ) );
  s->append( new Word( getArgs( "text='zie', " XML_ID"='" + corDoc.id() + ".s.1.w.2'" ),
		       &corDoc ));
  s->append( new Word( getArgs( "text='een', " XML_ID"='" + corDoc.id() + ".s.1.w.3'" ),
		       &corDoc ));
  s->append( new Word( getArgs("text='groot', " XML_ID"='" + corDoc.id() + ".s.1.w.4'" ),
		       &corDoc ));
  s->append( new Word( getArgs("text='huis', " XML_ID"='" + corDoc.id() + ".s.1.w.5'" ),
		       &corDoc ));
  s->append( new Word( getArgs("text='.', " XML_ID"='" + corDoc.id() + ".s.1.w.6'" ),
		       &corDoc ));

  s->deleteword( corDoc.index( corDoc.id() + ".s.1.w.4" ) );
  //  assertNoThrow( corDoc.save( "/tmp/foliadelete003.xml" ) );
  assertEqual( s->text(), "Ik zie een huis ." );
  assertEqual( s->xmlstring(), "<s xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example.s.1\"><w xml:id=\"example.s.1.w.1\"><t>Ik</t></w><w xml:id=\"example.s.1.w.2\"><t>zie</t></w><w xml:id=\"example.s.1.w.3\"><t>een</t></w><correction xml:id=\"example.s.1.correction.1\"><original auth=\"no\"><w xml:id=\"example.s.1.w.4\"><t>groot</t></w></original></correction><w xml:id=\"example.s.1.w.5\"><t>huis</t></w><w xml:id=\"example.s.1.w.6\"><t>.</t></w></s>");
}

void correction_test004(){
  startTestSerie( " Insert corrections " );
  Document corDoc( XML_ID"='example'" );
  assertNoThrow( corDoc.declare( AnnotationType::TOKEN,
				  "adhocset",
				  "annotator='proycon'" ) );
  Text *text = new Text( getArgs(XML_ID"='" + corDoc.id() + ".text.1'") );
  assertNoThrow( corDoc.addText( text ) );
  FoliaElement *s = text->append( new Sentence( getArgs(XML_ID"='" + corDoc.id() + ".s.1'"	) ) );
  s->append( new Word( getArgs("text='Ik', " XML_ID"='" + corDoc.id() + ".s.1.w.1'" ),
		       &corDoc ));
  s->append( new Word( getArgs("text='zie', " XML_ID"='" + corDoc.id() + ".s.1.w.2'"),
		       &corDoc ));
  s->append( new Word( getArgs( "text='een', " XML_ID"='" + corDoc.id() + ".s.1.w.3'" ),
		       &corDoc ));
  s->append( new Word( getArgs("text='huis', " XML_ID"='" + corDoc.id() + ".s.1.w.4'" ),
		       &corDoc ));
  s->append( new Word( getArgs("text='.', " XML_ID"='" + corDoc.id() + ".s.1.w.5'" ),
		       &corDoc ));

  Word *w = new Word( getArgs( XML_ID"='" + corDoc.id() + ".s.1.w.3b', text='groot'"), &corDoc );
  FoliaElement *w2 = 0;
  assertNoThrow( w2 = corDoc.index( corDoc.id() + ".s.1.w.3" ) );
  assertNoThrow( s->insertword( w, w2 ) );

  //  assertNoThrow( corDoc.save( "/tmp/foliainsert004.xml" ) );
  assertEqual( s->words().size(), 6 );
  assertEqual( s->text(), "Ik zie een groot huis ." );
  assertEqual( s->xmlstring(), "<s xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example.s.1\"><w xml:id=\"example.s.1.w.1\"><t>Ik</t></w><w xml:id=\"example.s.1.w.2\"><t>zie</t></w><w xml:id=\"example.s.1.w.3\"><t>een</t></w><correction xml:id=\"example.s.1.correction.1\"><new><w xml:id=\"example.s.1.w.3b\"><t>groot</t></w></new><original auth=\"no\"/></correction><w xml:id=\"example.s.1.w.4\"><t>huis</t></w><w xml:id=\"example.s.1.w.5\"><t>.</t></w></s>" );
}

void correction_test005(){
  startTestSerie( " Re-using a correction with only suggestions " );
  Document *corDoc = new Document();
  corDoc->readFromFile( "tests/example.xml" );
  FoliaElement *w = corDoc->index("WR-P-E-J-0000000001.p.1.s.8.w.11"); // stippelijn
  assertNoThrow( w->correct("suggestion='stippellijn', set='corrections', class='spelling',annotator='testscript', annotatortype='auto'" ) );
  //  assertNoThrow( corDoc->save( "/tmp/foliainsert005-1.xml" ) );
  FoliaElement *c = 0;
  assertNoThrow( c = w->annotation<Correction>() );
  assertEqual( c->suggestions(0)->text(), "stippellijn" );
  assertEqual( w->text(), "stippelijn" );

  assertNoThrow( w->correct("new='stippellijn', set='corrections', class='spelling',annotator='John Doe', annotatortype='manual', reuse='" + c->id() + "'" ) );

  assertEqual( w->text(), "stippellijn" );
  assertEqual( len(w->annotations<Correction>()), 1 );
  assertEqual( w->annotation<Correction>()->suggestions(0)->text(), "stippellijn" );
  assertEqual( w->annotation<Correction>()->suggestions(0)->annotator(), "testscript" );
  assertEqual( w->annotation<Correction>()->suggestions(0)->annotatortype(), AUTO );
  assertEqual( w->annotation<Correction>()->getNew()->text(), "stippellijn" );
  assertEqual( w->annotation<Correction>()->annotator(), "John Doe" );
  assertEqual( w->annotation<Correction>()->annotatortype(), MANUAL );

  assertEqual( w->xmlstring(), "<w xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.11\"><pos class=\"FOUTN(soort,ev,basis,zijd,stan)\" set=\"https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn\"/><lemma class=\"stippelijn\"/><correction xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.11.correction.1\" annotator=\"John Doe\" class=\"spelling\"><suggestion annotator=\"testscript\" annotatortype=\"auto\" auth=\"no\"><t>stippellijn</t></suggestion><new><t>stippellijn</t></new><original auth=\"no\"><t>stippelijn</t></original></correction></w>" );
  delete corDoc;
}

void correction_test006(){
  startTestSerie( "Correction - Suggestion for deletion with parent merge suggestion" );
  Document *corDoc = new Document();
  corDoc->readFromFile( "tests/example.xml" );
  KWargs args;
  args[XML_ID] = corDoc->id() + ".s.1";
  Sentence *sent = new Sentence( args );
  FoliaElement *text = 0;
  assertNoThrow( text = (*corDoc)[0] );
  assertNoThrow( text->append( sent ) );
  args[XML_ID] = corDoc->id() + ".s.1.w.1";
  args["text"] = "De";
  Word *wrd = new Word( args );
  assertNoThrow( sent->append( wrd ) );
  args[XML_ID] = corDoc->id() + ".s.1.w.2";
  args["text"] = "site";
  wrd = new Word( args );
  assertNoThrow( sent->append( wrd ) );
  args[XML_ID] = corDoc->id() + ".s.1.w.3";
  args["text"] = "staat";
  wrd = new Word( args );
  assertNoThrow( sent->append( wrd ) );
  args[XML_ID] = corDoc->id() + ".s.1.w.4";
  args["text"] = "on";
  wrd = new Word( args );
  assertNoThrow( sent->append( wrd ) );
  args[XML_ID] = corDoc->id() + ".s.1.w.5";
  args["text"] = "line";
  wrd = new Word( args );
  assertNoThrow( sent->append( wrd ) );
  args[XML_ID] = corDoc->id() + ".s.1.w.6";
  args["text"] = ".";
  wrd = new Word( args );
  assertNoThrow( sent->append( wrd ) );
  args.clear();
  args[XML_ID] = corDoc->id() + ".s.2";
  sent = new Sentence( args);
  text->append( sent );
  args[XML_ID] = corDoc->id() + ".s.2.w.1";
  args["text"] = "sinds";
  wrd = new Word( args );
  assertNoThrow( sent->append( wrd ) );
  args[XML_ID] = corDoc->id() + ".s.2.w.2";
  args["text"] = "vorige";
  wrd = new Word( args );
  assertNoThrow( sent->append( wrd ) );
  args[XML_ID] = corDoc->id() + ".s.2.w.3";
  args["text"] = "week";
  wrd = new Word( args );
  assertNoThrow( sent->append( wrd ) );
  args[XML_ID] = corDoc->id() + ".s.2.w.4";
  args["text"] = "zondag";
  wrd = new Word( args );
  assertNoThrow( sent->append( wrd ) );
  args[XML_ID] = corDoc->id() + ".s.2.w.5";
  args["text"] = ".";
  wrd = new Word( args );
  assertNoThrow( sent->append( wrd ) );

  FoliaElement *s = corDoc->index(corDoc->id() + ".s.1");
  FoliaElement *s2 = corDoc->index(corDoc->id() + ".s.2");
  FoliaElement *w = corDoc->index(corDoc->id() + ".s.1.w.6");
  s->remove(w,false); // don't delete w, we gonna re-use it
  Correction *corr = new Correction();
  assertNoThrow( s->append( corr ) );
  Current *cur = new Current();
  assertNoThrow( cur->append(w) );
  assertNoThrow( corr->append(cur) );
  args.clear();
  args["merge"] = s2->id();
  Suggestion *sug = 0;
  assertNoThrow( sug = new Suggestion( args ) );
  assertNoThrow( corr->append(sug) );

  assertEqual( s->xmlstring(),
	       "<s xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.s.1\"><w xml:id=\"WR-P-E-J-0000000001.s.1.w.1\"><t>De</t></w><w xml:id=\"WR-P-E-J-0000000001.s.1.w.2\"><t>site</t></w><w xml:id=\"WR-P-E-J-0000000001.s.1.w.3\"><t>staat</t></w><w xml:id=\"WR-P-E-J-0000000001.s.1.w.4\"><t>on</t></w><w xml:id=\"WR-P-E-J-0000000001.s.1.w.5\"><t>line</t></w><correction><current><w xml:id=\"WR-P-E-J-0000000001.s.1.w.6\"><t>.</t></w></current><suggestion auth=\"no\" merge=\"WR-P-E-J-0000000001.s.2\"/></correction></s>" );
  delete corDoc;
}

void correction_test007(){
  startTestSerie( " corrections on elements without an ID " );
  Document corDoc;
  corDoc.readFromFile( "tests/corr_str.xml" );
  assertNoThrow( corDoc.declare( AnnotationType::CORRECTION,
				 "corrections" ) );
  FoliaElement *p = corDoc.index("p.4");
  FoliaElement *w = p->index(8);
  assertNoThrow( w->correct("new='behandeling', set='corrections', class='spelling',annotator='testscript', annotatortype='auto'" ) );
  assertEqual( w->text(), "behandeling" );
  w = p->index(12);
  assertNoThrow( w->correct("new='Wijziging', set='corrections', class='spelling',annotator='John Doe', annotatortype='manual'" ) );
  assertEqual( w->text(), "Wijziging" );
}


Document qDoc( "file='tests/example.xml'" );

void query_test001(){
  startTestSerie( " Find Words (simple) " );
  vector<string> words = { "van", "het", "alfabet" };
  vector<vector<Word*> >matches = qDoc.findwords( Pattern(words) );
  assertEqual( matches.size(), 1 );
  assertEqual( len(matches[0]), 3 );
  assertEqual( matches[0][0]->text(), "van" );
  assertEqual( matches[0][1]->text(), "het" );
  assertEqual( matches[0][2]->text(), "alfabet" );
}

void query_test002(){
  startTestSerie( " Find Words (with wildcard) ");
  vector<string> words = { "van", "het", "*:1" };
  vector<vector<Word*> >matches = qDoc.findwords( Pattern(words) );
  assertEqual( matches.size(), 1 );
  assertEqual( len(matches[0]), 3 );

  assertEqual( matches[0][0]->text(), "van" );
  assertEqual( matches[0][1]->text(), "het" );
  assertEqual( matches[0][2]->text(), "alfabet" );
}

void query_test003(){
  startTestSerie( " Find Words by annotation " );
  vector<string> words = { "de", "historisch", "wetenschap", "worden" };
  vector<vector<Word*> >matches = qDoc.findwords( Pattern( words,
							   LemmaAnnotation_t ) );
  assertEqual( matches.size(), 1 );
  assertEqual( len(matches[0]), 4 );

  assertEqual( matches[0][0]->text(),"de" );
  assertEqual( matches[0][1]->text(), "historische" );
  assertEqual( matches[0][2]->text(), "wetenschap" );
  assertEqual( matches[0][3]->text(), "wordt" );
}

void query_test004(){
  startTestSerie( " Find Words using multiple patterns " );
  Pattern p1( { "de", "historische", "*", "wordt" } );
  Pattern p2( { "de", "historisch", "wetenschap", "worden" }, LemmaAnnotation_t );
  list<Pattern> l;
  l.push_back( p1 );
  l.push_back( p2 );
  vector<vector<Word*> >matches = qDoc.findwords( l );
  assertEqual( matches.size(), 1 );
  assertEqual( len(matches[0]), 4 );

  assertEqual( matches[0][0]->text(), "de" );
  assertEqual( matches[0][1]->text(), "historische" );
  assertEqual( matches[0][2]->text(), "wetenschap" );
  assertEqual( matches[0][3]->text(), "wordt" );
}

void query_test005(){
  startTestSerie( " Find Words that aren't there " );
  vector<vector<Word*> >matches
    = qDoc.findwords( Pattern({"bli","bla","blue"}) );
  assertEqual( matches.size(), 0 );
}

void query_test006(){
  startTestSerie( " Find words with overlap " );
  Document doc( XML_ID"='test'" );
  Text *text = new Text( getArgs(XML_ID"='test.text'") );
  doc.addText( text );
  FoliaElement *s = new Sentence( getArgs( XML_ID"='" + doc.id() + ".s.1'") );
  text->append( s );
  s->addWord( "text='a'" );
  s->addWord( "text='a'" );
  s->addWord( "text='a'" );
  s->addWord( "text='A'" ); // test caseinsensitivness too
  s->addWord( "text='b'" );
  s->addWord( "text='a'" );
  s->addWord( "text='a'" );

  Pattern pat1( {"a","a"} );
  vector<vector<Word*> > matches = doc.findwords( pat1 );
  assertEqual( matches.size(), 4 );
  Pattern pat2( {"a","a"}, "casesensitive='1'" );
  matches = doc.findwords( pat2 );
  assertEqual( matches.size(), 3 );
}

void query_test007(){
  startTestSerie( " Find Words with context " );
  Pattern p( {"van", "het", "alfabet" } );
  vector<vector<Word*> >matches
    = qDoc.findwords( p, "leftcontext='3', rightcontext='3'");
  assertEqual( matches.size(), 1 );
  assertEqual( len(matches[0]), 9 );

  assertEqual( matches[0][0]->text(), "de" );
  assertEqual( matches[0][1]->text(), "laatste" );
  assertEqual( matches[0][2]->text(), "letters" );
  assertEqual( matches[0][3]->text(), "van" );
  assertEqual( matches[0][4]->text(), "het" );
  assertEqual( matches[0][5]->text(), "alfabet" );
  assertEqual( matches[0][6]->text(), "en" );
  assertEqual( matches[0][7]->text(), "worden" );
  assertEqual( matches[0][8]->text(), "tussen" );
}

void query_test008(){
  startTestSerie( " Find Words with 'disjunction' (by using regexp) ");
  vector<string> words = { "de", "regexp('historische|hedendaagse')",
			   "wetenschap", "wordt" };
  vector<vector<Word*> >matches = qDoc.findwords( Pattern(words) );
  assertEqual( matches.size(), 1 );
  assertEqual( len(matches[0]), 4 );

  assertEqual( matches[0][0]->text(), "de" );
  assertEqual( matches[0][1]->text(), "historische" );
  assertEqual( matches[0][2]->text(), "wetenschap" );
  assertEqual( matches[0][3]->text(), "wordt" );
}

void query_test009(){
  startTestSerie( " Find Words with regular expressions " );
  vector<string> words = { "de", "regexp('hist.*')", "regexp('.*schap')",
			   "regexp('w[oae]rdt')" };
  vector<vector<Word*> >matches = qDoc.findwords( Pattern(words) );
  assertEqual( matches.size(), 1 );
  assertEqual( len(matches[0]), 4 );

  assertEqual( matches[0][0]->text(), "de" );
  assertEqual( matches[0][1]->text(), "historische" );
  assertEqual( matches[0][2]->text(), "wetenschap" );
  assertEqual( matches[0][3]->text(), "wordt" );
}

void query_test010a(){
  startTestSerie( " Find Words with variable wildcard " );
  vector<string> words = { "de", "laatste", "*", "alfabet" };
  vector<vector<Word*> >matches = qDoc.findwords( Pattern(words) );
  assertEqual( matches.size(), 1 );
  assertEqual( len(matches[0]), 6 );

  assertEqual( matches[0][0]->text(), "de" );
  assertEqual( matches[0][1]->text(), "laatste" );
  assertEqual( matches[0][2]->text(), "letters" );
  assertEqual( matches[0][3]->text(), "van" );
  assertEqual( matches[0][4]->text(), "het" );
  assertEqual( matches[0][5]->text(), "alfabet" );
}

void query_test010b(){
  startTestSerie( " Find words with wildcard and overlap " );
  Document doc( XML_ID"='test'" );
  Text *text = new Text( getArgs(XML_ID"='test.text'") );
  doc.addText( text );
  FoliaElement *s = new Sentence( getArgs(XML_ID"='" + doc.id() + ".s.1'") );
  text->append( s );
  s->addWord( "text='a'" );
  s->addWord( "text='b'" );
  s->addWord( "text='c'" );
  s->addWord( "text='d'" );
  s->addWord( "text='a'" );
  s->addWord( "text='b'" );
  s->addWord( "text='c'" );
  vector<vector<Word*> > matches = doc.findwords( Pattern({"a","*","c"}) );
  assertEqual( matches.size(), 3 );
}

void query_test011(){
  startTestSerie( " Find Words by non existing annotation " );
  vector<string> words = { "bli", "bla", "blu" };
  vector<vector<Word*> >matches = qDoc.findwords( Pattern(words,
							  SenseAnnotation_t ) );
  assertEqual( matches.size(), 0 );
}

#if FOLIA_INT_VERSION >= 115

void build_test001(){
  startTestSerie( " build a text document using FoliaBuilder " );
  ofstream os( "/tmp/build.xml" );
  Builder b( os, "build1" );
  KWargs args;
  args[XML_ID] = "div1";
  Division *d = new Division( args, b.doc() );
  args[XML_ID] = "p1";
  Paragraph *p = new Paragraph( args );
  d->append(p);
  p->settext( "paragraaf 1" );
  b.add( d );
  b.flush();
  args[XML_ID] = "div2";
  d = new Division( args );
  args[XML_ID] = "p2";
  p = new Paragraph( args );
  p->settext( "paragraaf 2" );
  d->append(p);
  b.add( d );
  b.flush();
  args[XML_ID] = "div3";
  d = new Division( args );
  args[XML_ID] = "p3";
  p = new Paragraph( args );
  p->settext( "paragraaf 3" );
  d->append(p);
  b.add( d );
  b.finish();
  int stat = system( "./tests/foliadiff.sh /tmp/build.xml tests/build.xml" );
  assertMessage( "/tmp/build.xml tests/build.xml differ!",
   		 (stat == 0) );
}

void build_test002(){
  startTestSerie( " build a speech document using FoliaBuilder " );
  ofstream os( "/tmp/speechbuild.xml" );
  Builder b( os, "build1", Builder::SPEECH );
  KWargs args;
  args[XML_ID] = "div1";
  Division *d = new Division( args, b.doc() );
  args[XML_ID] = "p1";
  Paragraph *p = new Paragraph( args );
  d->append(p);
  p->settext( "paragraaf 1" );
  b.add( d );
  b.flush();
  args[XML_ID] = "div2";
  d = new Division( args );
  args[XML_ID] = "p2";
  p = new Paragraph( args );
  p->settext( "paragraaf 2" );
  d->append(p);
  b.add( d );
  b.flush();
  args[XML_ID] = "div3";
  d = new Division( args );
  args[XML_ID] = "p3";
  p = new Paragraph( args );
  p->settext( "paragraaf 3" );
  d->append(p);
  b.add( d );
  b.finish();
  int stat = system( "./tests/foliadiff.sh /tmp/speechbuild.xml tests/speechbuild.xml" );
  assertMessage( "/tmp/speechbuild.xml tests/speechbuild.xml differ!",
   		 (stat == 0) );
}

void processor_test001a(){
  startTestSerie( " copy a document using Folia Processor " );
  Processor proc;
  // proc.set_debug(true);
  assertNoThrow( proc.init_doc( "tests/example.xml",
				"/tmp/example-p1.xml") );
  if ( proc.ok() ){
    FoliaElement *res = 0;
    while ( (res = proc.get_node( "pqrs" ) ) ){
      // search a non-existing node. makes get_node collect the whole document
      proc.next();
    }
    proc.finish();
    int stat = system( "./tests/foliadiff.sh /tmp/example-p1.xml tests/example.xml" );
    assertMessage( "/tmp/example-p1.xml tests/example.xml differ!",
     		   (stat == 0) );
  }
}

void processor_test001b(){
  startTestSerie( " copy a document with namespaces using Folia Processor" );
  Processor proc;
  //  proc.set_debug(true);
  assertNoThrow( proc.init_doc( "tests/folia.nsexample", "/tmp/nsexample" ) );
  if ( proc.ok() ){
    FoliaElement *res = 0;
    while ( (res = proc.get_node( "pqrs" ) ) ){
      // search a non-existing node. makes get_node collect the whole document
      proc.next();
    }
    assertNoThrow( proc.finish() );
    int stat = system( "./tests/foliadiff.sh /tmp/nsexample tests/folia.nsexample" );
    assertMessage( "/tmp/nsexample tests/folia.nsexample differ!",
     		   (stat == 0) );
  }
}

void processor_test001c(){
  startTestSerie( " process an invalid document using Folia Processor" );
  Processor proc;
  //  proc.set_debug(true);
  assertThrow( proc.init_doc( "tests/wrongname.xml" ), XmlError );
}

void loop( Processor& proc ){
  FoliaElement *res;
  while ( ( res = proc.get_node( "pqrs" ) ) ){
    // search a non-existing node. will barf on the unknown tag
    proc.next();
  }
}

void processor_test001d(){
  startTestSerie( " process an invalid document using Folia Processor" );
  Processor proc;
  //  proc.set_debug(true);
  assertNoThrow( proc.init_doc( "tests/unknowntag.xml" ) );
  if ( proc.ok() ){
    // init should work!
    // reading on should fail at a certain point
    assertThrow( loop( proc ), XmlError );
  }
}

void processor_test001e(){
  startTestSerie( " copy a difficult document using Folia Processor " );
  Processor proc;
  //  proc.set_debug(true);
  assertNoThrow( proc.init_doc( "tests/bug-1.xml",
				"/tmp/bug-1.xml") );
  if ( proc.ok() ){
    FoliaElement *res = 0;
    while ( (res = proc.get_node( "pqrs" ) ) ){
      // search a non-existing node. makes get_node collect the whole document
      proc.next();
    }
    proc.finish();
    int stat = system( "./tests/foliadiff.sh /tmp/bug-1.xml tests/bug-1.xml" );
    assertMessage( "/tmp/bug-1.xml tests/bug-1.xml differ!",
		   (stat == 0) );
  }
}


void processor_test002a(){
  startTestSerie( " copy a document using Folia Processor (alternative)" );
  Processor proc;
  assertNoThrow( proc.init_doc( "tests/example.xml" ) );
  if ( proc.ok() ){
    FoliaElement *res = 0;
    while ( (res = proc.get_node( "pqrs" ) ) ){
      // search a non-existing node. makes get_node collect the whole document
      proc.next();
    }
    assertNoThrow( proc.save( "/tmp/example-p2.xml") );
    int stat = system( "./tests/foliadiff.sh /tmp/example-p2.xml tests/example.xml" );
    assertMessage( "/tmp/example-p2.xml tests/example.xml differ!",
     		   (stat == 0) );
  }
}

void processor_test002b(){
  startTestSerie( " copy a document with namespace using Folia Processor (alternative)" );
  Processor proc;
  //  proc.set_debug(true);
  assertNoThrow( proc.init_doc( "tests/folia.nsexample" ) );
  if ( proc.ok() ){
    FoliaElement *res = 0;
    while ( (res = proc.get_node( "pqrs" ) ) ){
      // search a non-existing node. makes get_node collect the whole document
      proc.next();
    }
    assertNoThrow( proc.save( "/tmp/nsexample2.xml") );
    int stat = system( "./tests/foliadiff.sh /tmp/nsexample2.xml tests/folia.nsexample" );
    assertMessage( "/tmp/nsexample2.xml tests/folia.nsexample differ!",
     		   (stat == 0) );
  }
}

void processor_test002c(){
  startTestSerie( " copy a speech document using Folia Processor" );
  Processor proc;
  //  proc.set_debug(true);
  assertNoThrow( proc.init_doc( "tests/speechexample.xml" ) );
  if ( proc.ok() ){
    FoliaElement *res = 0;
    while ( (res = proc.get_node( "pqrs" ) ) ){
      // search a non-existing node. makes get_node collect the whole document
      proc.next();
    }
    assertNoThrow( proc.save( "/tmp/speechexample2.xml") );
    int stat = system( "./tests/foliadiff.sh /tmp/speechexample2.xml tests/speechexample.xml" );
    assertMessage( "/tmp/speechexample2.xml tests/speechexample.xml differ!",
     		   (stat == 0) );
  }
}

void processor_test003(){
  startTestSerie( " read a document and adding word and annotations using Foliaprocessor " );
  Processor proc;
  assertNoThrow( proc.init_doc( "tests/zin.xml", "/tmp/zin.xml") );
  if ( proc.ok() ){
    proc.declare( AnnotationType::POS,
		  "MY_TEST",
		  "annotator='foliatest', annotatortype='auto'" );
    FoliaElement *res = 0;
    vector<string> words({"aap","noot","mies"});
    int i = 0;
    while ( (res = proc.get_node( "s" ) ) ){
      Word *w = 0;
      assertNoThrow( w = res->addWord() );
      w->settext(words[i++]);
      KWargs args;
      args["set"] = "MY_TEST";
      args["class"] = "N";
      w->addPosAnnotation(args);
      proc.next();
    }
    proc.finish();
    int stat = system( "./tests/foliadiff.sh /tmp/zin.xml tests/zin.ok" );
    assertMessage( "/tmp/zin.xml tests/zin.ok differ!",
     		   (stat == 0) );
  }
}

void processor_test004(){
  startTestSerie( " read a document searching for several nodes " );
  Processor proc;
  //  proc.set_debug(true);
  assertNoThrow( proc.init_doc( "tests/zin.xml", "/tmp/zin2.xml") );
  if ( proc.ok() ){
    proc.declare( AnnotationType::POS,
		  "MY_TEST",
		  "annotator='foliatest', annotatortype='auto'" );
    FoliaElement *res = 0;
    while ( (res = proc.get_node( "s|p" ) ) ){
      string tag = res->xmltag();
      if ( tag == "p" ){
	vector<Sentence*> sv = res->select<Sentence>();
	for ( const auto& s : sv ){
	  Word *w = 0;
	  KWargs args;
	  args["text"] = "found: " + tag + "|s";
	  assertNoThrow( w = s->addWord( args ) );
	}
      }
      else {
	Word *w = 0;
	assertNoThrow( w = res->addWord() );
	w->settext( "found:" + tag );
      }
      proc.next();
    }
    proc.finish();
    int stat = system( "./tests/foliadiff.sh /tmp/zin2.xml tests/zin2.ok" );
    assertMessage( "/tmp/zin2.xml tests/zin2.ok differ!",
     		   (stat == 0) );
  }
}

void processor_test005(){
  startTestSerie( " enumerate a document on xml_element nodes " );
  Processor proc;
  //  proc.set_debug(true);
  assertNoThrow( proc.init_doc( "tests/example.xml" ) );
  if ( proc.ok() ){
    xml_tree *result = proc.create_simple_tree("tests/example.xml");
    ofstream os( "/tmp/enum.tree" );
    print( os, result );
    delete result;
    int stat = system( "diff /tmp/enum.tree tests/enum.tree.ok" );
    assertMessage( "/tmp/enum.tree tests/enum.tree.ok differ!",
     		   (stat == 0) );
  }
}

void processor_test006a(){
  startTestSerie( " enumerate a document on text node parents" );
  TextProcessor proc;
  //  proc.set_debug(true);
  assertNoThrow( proc.init_doc( "tests/example.xml" ) );
  if ( proc.ok() ){
    map<int,int> result = proc.enumerate_text_parents();
    ofstream os( "/tmp/textparents.lst" );
    for ( const auto& n : result ){
      os << n.first << endl;
    }
    int stat = system( "diff /tmp/textparents.lst tests/textparents.lst.ok" );
    assertMessage( "/tmp/textparents.lst tests/textparents.lst.ok differ!",
     		   (stat == 0) );
  }
}

void processor_test006b(){
  startTestSerie( " enumerate a document on text node parents with textclass" );
  TextProcessor proc;
  //  proc.set_debug(true);
  assertNoThrow( proc.init_doc( "tests/example.xml" ) );
  if ( proc.ok() ){
    map<int,int> result = proc.enumerate_text_parents("original");
    ofstream os( "/tmp/textparents-o.lst" );
    for ( const auto& n : result ){
      os << n.first << endl;
    }
    int stat = system( "diff /tmp/textparents-o.lst tests/textparents-o.lst.ok" );
    assertMessage( "/tmp/textparents-o.lst tests/textparents-o.lst.ok differ!",
     		   (stat == 0) );
  }
}

void processor_test006c(){
  startTestSerie( " enumerate a document with layers on text node parents" );
  TextProcessor proc;
  //  proc.set_debug(true);
  assertNoThrow( proc.init_doc( "tests/example_1.xml" ) );
  if ( proc.ok() ){
    proc.setup();
    map<int,int> result = proc.enumerate_text_parents(""); // silly, redoing enumerate
    ofstream os( "/tmp/textparents-1.lst" );
    for ( const auto& n : result ){
      os << n.first << endl;
    }
    int stat = system( "diff /tmp/textparents-1.lst tests/textparents-1.lst.ok" );
    assertMessage( "/tmp/textparents-1.lst tests/textparents-1.lst.ok differ!",
     		   (stat == 0) );
    ofstream os2( "/tmp/textparents-1-tag.lst" );
    FoliaElement *e=0;
    while ( (e = proc.next_text_parent() ) ){
      os2 << e->xmltag() << " : " << e->str() << endl;
    }
    stat = system( "diff /tmp/textparents-1-tag.lst tests/textparents-1-tag.lst.ok" );
    assertMessage( "/tmp/textparents-1-tag.lst tests/textparents-1-tag.lst.ok differ!",
     		   (stat == 0) );

  }
}

void processor_test006d(){
  startTestSerie( " enumerate a complex document on text node parents" );
  TextProcessor proc;
  //  proc.set_debug(true);
  assertNoThrow( proc.init_doc( "tests/folia-head.xml" ) );
  if ( proc.ok() ){
    map<int,int> result = proc.enumerate_text_parents("");
    ofstream os( "/tmp/textparents-2.lst" );
    for ( const auto& n : result ){
      os << n.first << endl;
    }
    int stat = system( "diff /tmp/textparents-2.lst tests/textparents-2.lst.ok" );
    assertMessage( "/tmp/textparents-2.lst tests/textparents-2.lst.ok differ!",
     		   (stat == 0) );
  }
}

void processor_test007(){
  startTestSerie( " process a document searching for text nodes " );
  TextProcessor proc;
  //  proc.set_debug(true);
  assertNoThrow( proc.init_doc( "tests/zin.xml", "/tmp/zin3.xml") );
  if ( proc.ok() ){
    proc.setup();
    proc.declare( AnnotationType::POS,
		  "MY_TEST",
		  "annotator='foliatest', annotatortype='auto'" );
    FoliaElement *w = 0;
    char character = 'A';
    while ( (w = proc.next_text_parent() ) ){
      // we know we should get only <w> nodes for zin.xml....
      KWargs args;
      args["set"] = "MY_TEST";
      args["class"] = string(1,character++);
      w->addPosAnnotation(args);
    }
    proc.finish();
    int stat = system( "./tests/foliadiff.sh /tmp/zin3.xml tests/zin3.ok" );
    assertMessage( "/tmp/zin3.xml tests/zin3.ok differ!",
     		   (stat == 0) );
  }
}

void processor_test008a(){
  startTestSerie( " process a document searching for text nodes " );
  TextProcessor proc;
  //  proc.set_debug(true);
  assertNoThrow( proc.init_doc( "tests/proctest.xml" ) );
  ofstream os( "/tmp/proctest.out" );
  if ( proc.ok() ){
    proc.setup("",true);
    FoliaElement *e = 0;
    while ( (e = proc.next_text_parent() ) ){
      os << e->id() << " : " << e->str() << endl;
      vector<Word*> wv = e->select<Word>();
      for ( const auto& w : wv ){
	os << "\t" << w->id() << " : " << w->str() << endl;
      }
    }
    int stat = system( "diff /tmp/proctest.out tests/proctest.ok" );
    assertMessage( "/tmp/proctest.out tests/proctest.ok differ!",
     		   (stat == 0) );
  }
}

void processor_test008b(){
  startTestSerie( " process a document with layers searching for text nodes " );
  TextProcessor proc;
  //  proc.set_debug(true);
  assertNoThrow( proc.init_doc( "tests/example_1.xml" ) );
  ofstream os( "/tmp/proctest-2.out" );
  if ( proc.ok() ){
    proc.setup("",true);
    FoliaElement *e = 0;
    while ( (e = proc.next_text_parent() ) ){
      os << e->id() << " : " << e->str() << endl;
      vector<Word*> wv = e->select<Word>();
      for ( const auto& w : wv ){
	os << "\t" << w->id() << " : " << w->str() << endl;
      }
    }
    int stat = system( "diff /tmp/proctest-2.out tests/proctest-2.ok" );
    assertMessage( "/tmp/proctest-2.out tests/proctest-2.ok differ!",
     		   (stat == 0) );
  }
}

void processor_test008c(){
  startTestSerie( " process a complex document searching for text nodes " );
  TextProcessor proc;
  //  proc.set_debug(true);
  assertNoThrow( proc.init_doc( "tests/folia-head.xml" ) );
  ofstream os( "/tmp/proctest-3.out" );
  if ( proc.ok() ){
    proc.setup("",true);
    FoliaElement *e = 0;
    while ( (e = proc.next_text_parent() ) ){
      os << e->id() << " : " << e->str() << endl;
      vector<Word*> wv = e->select<Word>();
      for ( const auto& w : wv ){
	os << "\t" << w->id() << " : " << w->str() << endl;
      }
    }
    int stat = system( "diff /tmp/proctest-3.out tests/proctest-3.ok" );
    assertMessage( "/tmp/proctest-3.out tests/proctest-3.ok differ!",
     		   (stat == 0) );
  }
}

#if FOLIA_INT_VERSION >= 116
void test_proc( TextProcessor& proc ){
  FoliaElement *e = 0;
  while ( (e = proc.next_text_parent() ) ){
  }
}

void processor_test008d(){
  startTestSerie( " process a strange document searching for text nodes " );
  TextProcessor proc( "tests/scary.xml", "/tmp/proctest-8d.xml");
  if ( proc.ok() ){
    proc.setup("",true);
    assertThrow( test_proc( proc ), XmlError );
  }
}
#else
void processor_test008d(){
  startTestSerie( " process a strange document searching for text nodes " );
  TextProcessor proc( "tests/scary.xml", "/tmp/proctest-8d.xml");
  // proc.set_debug(true);
  ofstream os( "/tmp/proctest-8d.out" );
  if ( proc.ok() ){
    proc.setup("",true);
    FoliaElement *e = 0;
    while ( (e = proc.next_text_parent() ) ){
      os << e->id() << " : " << e->str() << endl;
    }
    proc.finish();
    int stat = system( "diff /tmp/proctest-8d.out tests/proctest-8d.ok" );
    assertMessage( "/tmp/proctest-8d.out tests/proctest-8d.ok differ!",
     		   (stat == 0) );
    stat = system( "./tests/foliadiff.sh /tmp/proctest-8d.xml tests/proctest-8d.xml.ok" );
    assertMessage( "/tmp/proctest-8d.xml tests/proctest-8d.xml.ok differ!",
     		   (stat == 0) );
  }
}
#endif

void processor_test008e(){
  startTestSerie( " process a document with entities, searching for text nodes " );
  TextProcessor proc( "tests/entities.xml" );
  // proc.set_debug(true);
  ofstream os( "/tmp/entities.out" );
  if ( proc.ok() ){
    proc.setup("",true);
    FoliaElement *e = 0;
    while ( (e = proc.next_text_parent() ) ){
      os << e->id() << " : " << e->str() << endl;
    }
    int stat = system( "diff /tmp/entities.out tests/entities.ok" );
    assertMessage( "/tmp/entites.out tests/entities.ok differ!",
     		   (stat == 0) );
    proc.save( "/tmp/entities-p.xml" );
    stat = system( "./tests/foliadiff.sh /tmp/entities-p.xml tests/entities.xml" );
    assertMessage( "/tmp/entities-p.xml tests/entities.xml differ!",
     		   (stat == 0) );
  }
}

void processor_test009a(){
  startTestSerie( " process a difficult document with t-style " );
  TextProcessor proc( "tests/procbug.xml" );
  //  proc.set_debug(true);
  ofstream os( "/tmp/procbug.out" );
  if ( proc.ok() ){
    proc.setup("",true);
    FoliaElement *e = 0;
    assertNoThrow( e = proc.next_text_parent() );
    while ( e ) {
      os << e->id() << " : " << e->str() << endl;
      assertNoThrow( e = proc.next_text_parent() );
    }
    int stat = system( "diff /tmp/procbug.out tests/procbug.ok" );
    assertMessage( "/tmp/procbug.out tests/procbug.ok differ!",
     		   (stat == 0) );
  }
}

void processor_test009b(){
  startTestSerie( " process a difficult document for text " );
  TextProcessor proc( "tests/textbug1.xml" );
  //  proc.set_debug(true);
  ofstream os( "/tmp/textbug1.out" );
  if ( proc.ok() ){
    proc.setup("",true);
    FoliaElement *e = 0;
    assertNoThrow( e = proc.next_text_parent() );
    while ( e ) {
      os << e->id() << " : " << e->str() << endl;
      assertNoThrow( e = proc.next_text_parent() );
    }
    int stat = system( "diff /tmp/textbug1.out tests/textbug1.ok" );
    assertMessage( "/tmp/textbug1.out tests/textbug1.ok differ!",
     		   (stat == 0) );
  }
}

void processor_test009c(){
  startTestSerie( " process a difficult document for text " );
  TextProcessor proc;
  //  proc.set_debug(true);
  assertNoThrow( proc.init_doc( "tests/bug-2.xml" ) );
  ofstream os( "/tmp/textbug2.out" );
  if ( proc.ok() ){
    proc.setup("",true);
    FoliaElement *e = 0;
    assertNoThrow( e = proc.next_text_parent() );
    while ( e ) {
      os << e->id() << " : " << e->str() << endl;
      assertNoThrow( e = proc.next_text_parent() );
    }
    int stat = system( "diff /tmp/textbug2.out tests/textbug2.ok" );
    assertMessage( "/tmp/textbug2.out tests/testbug2.ok differ!",
		   (stat == 0) );
  }
}

void processor_test010(){
  startTestSerie( " copy a document using Folia Processor on string buffer" );
  string xml = "<?xml version=\"1.0\"?>\n"
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\""
" xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example\" generator=\"libfolia-v0.8\" version=\"0.8\">\n"
"  <metadata src=\"test.cmdi.xml\" type=\"cmdi\">\n"
"    <annotations>\n"
"      <pos-annotation set=\"test\"/>\n"
"    </annotations>\n"
"  </metadata>\n"
"  <text xml:id=\"test.text\">\n"
"    <div xml:id=\"div\">\n"
"     <head xml:id=\"head\">\n"
"       <s xml:id=\"head.1.s.1\">\n"
"            <w xml:id=\"head.1.s.1.w.1\">\n"
"                <t>blah</t>\n"
"                <pos class=\"NN(blah)\" head=\"NN\" />\n"
"            </w>\n"
"        </s>\n"
"    </head>\n"
"    <p xml:id=\"p.1\">\n"
"        <s xml:id=\"p.1.s.1\">\n"
"            <w xml:id=\"p.1.s.1.w.1\">\n"
"                <t>blah</t>\n"
"                <pos class=\"BB(blah)\">\n"
"                    <feat subset=\"head\" class=\"BB\" />\n"
"                </pos>\n"
"            </w>\n"
"        </s>\n"
"    </p>\n"
"   </div>\n"
"  </text>\n"
"</FoLiA>\n" ;

  Processor proc;
  // proc.set_debug(true);
  assertNoThrow( proc.init_doc( xml ) );
  if ( proc.ok() ){
    FoliaElement *res = 0;
    while ( (res = proc.get_node( "pqrs" ) ) ){
      // search a non-existing node. makes get_node collect the whole document
      proc.next();
    }
    assertNoThrow( proc.save( "/tmp/example-buffer.xml") );
    int stat = system( "./tests/foliadiff.sh /tmp/example-buffer.xml tests/example-buffer.ok" );
    assertMessage( "/tmp/example-buffer.xml tests/example-buffer.ok differ!",
     		   (stat == 0) );
  }
}

void processor_test011() {
  startTestSerie( " Test lezen en schrijven van een BZ2 FoLiA file " );
  Processor proc( "/tmp/example.xml.bz2" ); // created by earlier test
  while ( proc.get_node( "pqrs" ) ){
    // search a non-existing node. makes get_node collect the whole document
    proc.next();
  }
  proc.save( "/tmp/example-1.xml.bz2" );
  int stat = system( "./tests/foliadiff.sh /tmp/example-1.xml.bz2 /tmp/example.xml.bz2" );
  assertMessage( "/tmp/example-1.xml.bz2 tests/example.xml.bz2 differ!",
   		 (stat == 0) );
}

#endif // FOLIA_INT_VERSION >= 115

int main(){
  //processor_test006c();
  //exit(9);
  test0();
  test1();
  test1a();
  test1b();
  test1c();
  test1d();
#if FOLIA_INT_VERSION >= 115
  test1e();
#endif
#if FOLIA_INT_VERSION >= 114
  test1f();
#endif
  test2();
  test3();
  test4();
  test5();
  test6();
  test7();
  test8();
  test9();
  sanity_test000();
  sanity_test001();
  sanity_test002();
  sanity_test003a();
  sanity_test003b();
  sanity_test004();
  sanity_test005();
  sanity_test006a();
  sanity_test006b();
  sanity_test006c();
  sanity_test007();
  sanity_test008a();
  sanity_test008b();
  sanity_test008c();
  sanity_test009();
  sanity_test010();
  sanity_test011();
  sanity_test012();
  sanity_test013();
  sanity_test014();
  sanity_test015();
  sanity_test016a();
  sanity_test016b();
  sanity_test017();
  sanity_test018();
  sanity_test019();
  sanity_test020a();
  sanity_test020b();
  sanity_test020c();
  sanity_test020d();
  sanity_test020e();
  sanity_test020f();
  sanity_test020g();
  sanity_test021();
  sanity_test022();
  sanity_test023a();
  sanity_test023b();
  sanity_test023c();
  sanity_test024a();
  sanity_test024b();
  sanity_test024c();
  sanity_test025a();
  sanity_test025b();
  sanity_test025c();
  sanity_test026a();
  sanity_test026b();
  sanity_test027();
  sanity_test028();
  sanity_test029();
  sanity_test030();
  sanity_test030b();
  sanity_test031();
  sanity_test032();
  sanity_test033();
  sanity_test034();
  sanity_test035();
  sanity_test036();
  sanity_test037a();
  sanity_test037b();
  sanity_test037c();
  sanity_test038a();
  sanity_test038b();
  sanity_test039();
  sanity_test040();
  sanity_test041a();
  sanity_test041b();
  sanity_test041c();
  sanity_test042();
  sanity_test043();
  sanity_test044a();
  sanity_test044b();
  sanity_test045();
  sanity_test046();
  sanity_test046a();
  sanity_test046b();
  sanity_test047();
  sanity_test048();
  sanity_test049();
  sanity_test050();
  sanity_test099();
  sanity_test100a();
  sanity_test100b();
  sanity_test101();
  sanity_test101a();
  sanity_test101b();
  sanity_test101c();
  sanity_test101d();
  sanity_test101e();
  sanity_test101f();
  sanity_test101g();
  sanity_test102();
  sanity_test102a();
  sanity_test102b();
  sanity_test102c();
  sanity_test102d1();
  sanity_test102d2();
  sanity_test102d3();
  sanity_test102e();
  sanity_test102f();
  sanity_test102g();
  sanity_test102h();
  sanity_test102i();
  sanity_test102j();
  sanity_test102k();
  sanity_test102l();
  sanity_test102m();
  sanity_test102n();
  sanity_test102o();
  sanity_test103();
  sanity_test104a();
  sanity_test104b();
  sanity_test105();
  sanity_test106();
  sanity_test107();
  sanity_test108();
  sanity_test109();
  sanity_test110();
  sanity_test120();
  sanity_test121();
  sanity_test122();
  sanity_test123();
  edit_test001a();
  edit_test001b();
  edit_test002();
  edit_test003();
  edit_test003b();
  edit_test004a();
  edit_test004b();
  edit_test004c();
  edit_test005a();
  edit_test005b();
  edit_test005c();
  edit_test005d();
  edit_test006();
  edit_test007();
  edit_test008();
  edit_test009a();
  //  edit_test009b();
  edit_test009c();
  edit_test010();
  edit_test011();
  edit_test012();
  edit_test013();
  edit_test013b();
  edit_test013c();
  edit_test013d();
  edit_test014();
  edit_test015();
  edit_test016();
  edit_test017a();
  edit_test017b();
  edit_test018a();
  edit_test018b();
  edit_test018c();
  edit_test019();
  text_test03();
  text_test04();
  text_test05();
  text_test06();
  text_test07();
  text_test08();
  text_test09();
  text_test10();
  text_test11();
  text_test12();
  text_test13a();
  text_test13b();
  text_test13c();
  text_test13d();
  text_test13e();
  text_test13f();
  text_test14();
  text_test15();
  text_test16();
  text_test17();
  //  text_test18();
  create_test001();
  create_test002();
  create_test003();
  create_test004();
  correction_test001a();
  correction_test001b();
  correction_test002();
  correction_test003();
  correction_test004();
  correction_test005();
  correction_test006();
  correction_test007();
  query_test001();
  query_test002();
  query_test003();
  query_test004();
  query_test005();
  query_test006();
  query_test007();
  query_test008();
  query_test009();
  query_test010a();
  query_test010b();
  query_test011();
#if FOLIA_INT_VERSION >= 115
  build_test001();
  build_test002();
  processor_test001a();
  processor_test001b();
  processor_test001c();
  processor_test001d();
  processor_test001e();
  processor_test002a();
  processor_test002b();
  processor_test002c();
  processor_test003();
  processor_test004();
#if FOLIA_INT_VERSION > 115
  processor_test005();
  processor_test006a();
#endif
  processor_test006b();
#if FOLIA_INT_VERSION > 115
  processor_test006c();
#endif
  processor_test006d();
  processor_test007();
  processor_test008a();
  processor_test008b();
  processor_test008c();
  processor_test008d();
  processor_test008e();
#if FOLIA_INT_VERSION > 115
  processor_test009a();
  processor_test009b();
  processor_test009c();
  processor_test010();
  processor_test011();
#endif
#endif
  summarize_tests(0);
}
