/*
  Copyright (c) 2006 - 2020
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
#include "ticcutils/FileUtils.h"
#include "ticcutils/UnitTest.h"
#include "ticcutils/CommandLine.h"

#include "config.h"
using namespace std;
using namespace icu;
using namespace folia;

using TiCC::operator<<;

#define AlignReference LinkReference
#define Alignment Relation
#define ComplexAlignment SpanRelation
#define ComplexAlignmentLayer SpanRelationLayer
#define AbstractTokenAnnotation AbstractInlineAnnotation
#define AbstractTokenAnnotation_t AbstractInlineAnnotation_t

int expect = 0;

string default_path =  "../FoLiApy/folia-repo";
string legacy_file;
Document LEGACYEXAMPLE;

string get_folia_path() {
    string path;
    const char *env = getenv("FOLIAPATH");
    if ( env == NULL ){
      if ( TiCC::isDir( default_path + "/examples" ) ){
        env = default_path.c_str();
      }
      else {
        cerr << "WARNING: FOLIAPATH not set or guessed!!!" << endl;
        return "../FoLiA/";
      }
    };
    path = env;
    path += "/";
    return path;
}

string fol_path = get_folia_path();

bool setup(){
  legacy_file = fol_path + "examples/full-legacy.1.5.folia.xml";
  LEGACYEXAMPLE.read_from_file( legacy_file );
  const char *env = getenv("EXPECT");
  if ( env != NULL ){
    string value = env;
    if ( !TiCC::stringTo( value, expect ) ){
      cerr << "illegal value '" << value
	   << "' for EXPECT environment variable." << endl;
      return EXIT_FAILURE;
    }
  }
  return true;
}

void test0() {
  startTestSerie( " Test lezen van KWargs " );
  KWargs bla;
  assertNoThrow( bla.init( "dit='goed', dat =' ra ar' " ) );
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
  assertNoThrow( bla.init( "class='ok\\a', bli='bla'" ) );
  assertEqual( bla.is_present( "class" ), true );
  assertEqual( bla.is_present( "klas" ), false );
  assertEqual( bla.extract("class"), "ok\\a" );
  assertEqual( bla.extract("class"), "" );
  assertEqual( bla.extract("klas"), "" );
  assertTrue( bla["bli"] == "bla" );
  assertNoThrow( bla = getArgs( "cls='ok\\\\', bli='bla'" ) );
  assertTrue( bla["cls"] == "ok\\" );
  assertTrue( bla["bli"] == "bla" );
}


void test1() {
  startTestSerie( " Test lezen van een FoLiA file " );
  Document d;
  assertNoThrow( d.read_from_file( "tests/example.xml" ) );
  assertNoThrow( d.save( "/tmp/example.xml" ) );
  int stat = system( "./tests/foliadiff.sh /tmp/example.xml tests/example.xml" );
  assertMessage( "/tmp/example.xml tests/example.xml differ!",
   		 (stat == 0) );
}

void test1a() {
  startTestSerie( " Test lezen en schrijven van een BZ2 FoLiA file " );
  Document d1;
  assertNoThrow( d1.read_from_file( "tests/example.xml" ) );
  assertNoThrow( d1.save( "/tmp/example.xml.bz2" ) );
  Document d2;
  assertNoThrow( d2.read_from_file( "/tmp/example.xml.bz2" ) );
  assertNoThrow( d2.save( "/tmp/example.xml" ) );
  int stat = system( "./tests/foliadiff.sh /tmp/example.xml tests/example.xml" );
  assertMessage( "/tmp/example.xml tests/example.xml differ!",
   		 (stat == 0) );
}

void test1b() {
  startTestSerie( " Test lezen en schrijven van een GZ FoLiA file " );
  Document d1;
  assertNoThrow( d1.read_from_file( "tests/example.xml" ) );
  assertNoThrow( d1.save( "/tmp/example.xml.gz" ) );
  Document d2;
  assertNoThrow( d2.read_from_file( "/tmp/example.xml.gz" ) );
  assertNoThrow( d2.save( "/tmp/folia.gz.example" ) );
  int stat = system( "./tests/foliadiff.sh /tmp/folia.gz.example tests/example.xml" );
  assertMessage( "/tmp/folia.gz.example tests/example.xml differ!",
   		 (stat == 0) );
}

void test1c() {
  startTestSerie( " Test lezen van een FoLiA speech file " );
  Document d;
  assertNoThrow( d.read_from_file( "tests/speechexample.xml" ) );
  assertNoThrow( d.save( "/tmp/speechexample.xml" ) );
  int stat = system( "./tests/foliadiff.sh /tmp/speechexample.xml tests/speechexample.xml" );
  assertMessage( "/tmp/speechexample.xml tests/speechexample.xml differ!",
   		 (stat == 0) );
}

void test1d() {
  startTestSerie( " Test lezen van een FoLiA invalid file " );
  Document d;
  assertThrow( d.read_from_file( "tests/unknowntag.xml" ), XmlError );
}

void test1e() {
  startTestSerie( " Test lezen van een invalid FoLiA file " );
  Document d1;
  assertThrow( d1.read_from_file( "tests/scary.xml" ), XmlError );
}

void test1f() {
  startTestSerie( " Test lezen en schrijven van een FoLiA file met entities" );
  Document d1;
  assertNoThrow( d1.read_from_file( "tests/entities.xml" ) );
  Sentence *s = d1.sentences()[0];
  assertEqual( s->text(), "Dit <is> als het ware één test met ß." );
  assertNoThrow( d1.save( "/tmp/entities.xml" ) );
  int stat = system( "./tests/foliadiff.sh /tmp/entities.xml tests/entities.xml" );
  assertMessage( "/tmp/entities.xml tests/entities.xml differ!",
   		 (stat == 0) );
}

void test2() {
  startTestSerie( " Test lezen van een FoLiA string " );
  string s;
  ifstream is( "tests/example.xml" );
  string line;
  while( getline( is, line ) ){
    s += line + "\n";
  };
  Document d;
  assertNoThrow( d.read_from_string( s ) );
}

void test3() {
  startTestSerie( " Test lezen van een DCOI file " );
  Document d;
#if FOLIA_INT_VERSION >= 214
  assertThrow( d.read_from_file( "tests/dcoi.example" ), DocumentError );
#else
  assertThrow( d.read_from_file( "tests/dcoi.example" ), XmlError );
#endif
}

void test4() {
  startTestSerie( " Test uitvoer van een FoLiA file naar string" );
  Document d;
  assertNoThrow( d.read_from_file( "tests/example.xml" ) );
  string out;
  assertNoThrow( out = d.toXml() );
}

void test5() {
  startTestSerie( " Test lezen van een FoLiA file met namespaces" );
  Document d;
  assertNoThrow( d.read_from_file( "tests/folia.nsexample" ) );
  assertNoThrow( d.save( "/tmp/test5.out", "fl", false ) );
  int stat = system( "./tests/foliadiff.sh /tmp/test5.out tests/folia.nsexample" );
  assertMessage( "/tmp/test5.out tests/folia.nsexample differ!",
   		 (stat == 0) );
}

void test6() {
  startTestSerie( " Test lezen van een FoLiA file external nodes" );
  Document d;
  assertNoThrow( d.read_from_file( "tests/include1.xml" ) );
  assertNoThrow( d.save( "/tmp/include.out" ) );
  int stat = system( "./tests/foliadiff.sh /tmp/include.out tests/include.ok" );
  assertMessage( "/tmp/include.out tests/include.ok differ!",
   		 (stat == 0) );
  //Document d2;
  //assertThrow( d2.read_from_file( "tests/include2.xml" ), XmlError );
}

void test7() {
  startTestSerie( " Test inlezen van een FoLiA file zonder namespace declaratie" );
  Document d;
#if FOLIA_INT_VERSION >= 214
  assertThrow( d.read_from_file( "tests/noname.xml" ), DocumentError );
#else
  assertThrow( d.read_from_file( "tests/noname.xml" ), XmlError );
#endif
}

void test8() {
  startTestSerie( " Test inlezen van een FoLiA file met foute namespace declaratie" );
  Document d;
#if FOLIA_INT_VERSION >= 214
  assertThrow( d.read_from_file( "tests/wrongname.xml" ), DocumentError );
#else
  assertThrow( d.read_from_file( "tests/wrongname.xml" ), XmlError );
#endif
}

void test9() {
  startTestSerie( " Test extracting text from a document" );
  Document doc;
  assertNoThrow( doc.read_from_file( "tests/text.xml" ) );
  UnicodeString us;
  assertNoThrow( us = doc.text() );
  assertEqual( us, "chapter 1\n\nsentence 1" );
}

extern void sanity_test000();
extern void sanity_test001();
extern void sanity_test002();
extern void sanity_test003a();
extern void sanity_test003b();
extern void sanity_test004();
extern void sanity_test005();
extern void sanity_test006a();
extern void sanity_test006b();
extern void sanity_test006c();
extern void sanity_test007();
extern void sanity_test008a();
extern void sanity_test008b();
extern void sanity_test008c();
extern void sanity_test009();
extern void sanity_test010();
extern void sanity_test011();
extern void sanity_test012();
extern void sanity_test013();
extern void sanity_test014();
extern void sanity_test015();
extern void sanity_test016a();
extern void sanity_test016b();
extern void sanity_test017();
extern void sanity_test018();
extern void sanity_test019();
extern void sanity_test020a();
extern void sanity_test020b();
extern void sanity_test020c();
extern void sanity_test020d();
extern void sanity_test020e();
extern void sanity_test020f();
extern void sanity_test020g();
extern void sanity_test021();
extern void sanity_test022();
extern void sanity_test023a();
extern void sanity_test023b();
extern void sanity_test023c();
extern void sanity_test023();
extern void sanity_test024a();
extern void sanity_test024b();
extern void sanity_test024c();
extern void sanity_test025a();
extern void sanity_test025b();
extern void sanity_test025c();
extern void sanity_test026a();
extern void sanity_test026b();
extern void sanity_test027();
extern void sanity_test028();
extern void sanity_test029();
extern void sanity_test030();
extern void sanity_test030b();
extern void sanity_test031();
extern void sanity_test032();
extern void sanity_test033();
extern void sanity_test034();
extern void sanity_test035();
extern void sanity_test036();
extern void sanity_test037a();
extern void sanity_test037b();
extern void sanity_test037c();
extern void sanity_test038a();
extern void sanity_test038b();
extern void sanity_test039();
extern void sanity_test040();
extern void sanity_test041a();
extern void sanity_test041b();
extern void sanity_test041c();
extern void sanity_test042();
extern void sanity_test043();
extern void sanity_test044a();
extern void sanity_test044b();
extern void sanity_test045();
extern void sanity_test046();
extern void sanity_test046a();
extern void sanity_test046b();
extern void sanity_test047();
extern void sanity_test048();
extern void sanity_test049();
extern void sanity_test050();
extern void sanity_test099();
extern void sanity_test100();
extern void sanity_test101();
extern void sanity_test101a();
extern void sanity_test101b();
extern void sanity_test101c();
extern void sanity_test101d();
extern void sanity_test101e();
extern void sanity_test101f();
extern void sanity_test101g();
extern void sanity_test102();
extern void sanity_test102a();
extern void sanity_test102b();
extern void sanity_test102c();
extern void sanity_test102d1();
extern void sanity_test102d2();
extern void sanity_test102d3();
extern void sanity_test102e();
extern void sanity_test102f();
extern void sanity_test102g();
extern void sanity_test102h();
extern void sanity_test102i();
extern void sanity_test102j();
extern void sanity_test102k();
extern void sanity_test102l();
extern void sanity_test102m();
extern void sanity_test102n();
extern void sanity_test102o();
extern void sanity_test103();
extern void sanity_test104a();
extern void sanity_test104b();
extern void sanity_test105();
extern void sanity_test106();
extern void sanity_test107();
extern void sanity_test108();
extern void sanity_test109();
extern void sanity_test110();
extern void sanity_test120();
extern void sanity_test121();
extern void sanity_test122();
extern void sanity_test123();
extern void sanity_test130();
extern void sanity_test140();
extern void sanity_test141();
extern void sanity_test150();

extern void Test_E001_Tokens_Structure();
extern void Test_Exxx_Hidden_Tokens();
extern void Test_Exxx_Invalid_Wref();
extern void Test_Exxx_KeepVersion();
extern void Test_Exxx_SetAndSetLess();
extern void Test_Provenance();

extern void edit_test001a();
extern void edit_test001b();
extern void edit_test002();
extern void edit_test003();
extern void edit_test003b();
extern void edit_test004a();
extern void edit_test004b();
extern void edit_test004c();
extern void edit_test005();
extern void edit_test005a();
extern void edit_test005b();
extern void edit_test005c();
extern void edit_test005d();
extern void edit_test006();
extern void edit_test007();
extern void edit_test008();
extern void edit_test009a();
extern void edit_test009b();
extern void edit_test009c();
extern void edit_test010();
extern void edit_test011();
extern void edit_test012();
extern void edit_test013();
extern void edit_test013a();
extern void edit_test013b();
extern void edit_test013c();
extern void edit_test013d();
extern void edit_test014();
extern void edit_test015();
extern void edit_test016();
extern void edit_test017a();
extern void edit_test017b();
extern void edit_test018a();
extern void edit_test018b();
extern void edit_test018c();
extern void edit_test019();

extern void text_test01();
extern void text_test02();
extern void text_test03();
extern void text_test04();
extern void text_test05();
extern void text_test06();
extern void text_test07();
extern void text_test08();
extern void text_test08b();
extern void text_test09();
extern void text_test10();
extern void text_test11();
extern void text_test12();
extern void text_test13a();
extern void text_test13b();
extern void text_test13c();
extern void text_test13d();
extern void text_test13e();
extern void text_test13f();
extern void text_test14();
extern void text_test15();
extern void text_test16();
extern void text_test17();
extern void text_test18();
extern void text_test19a();
extern void text_test19b();
extern void text_test20();

extern void engine_test001a();
extern void engine_test001b();
extern void engine_test001c();
extern void engine_test001d();
extern void engine_test001e();
extern void engine_test001f();
extern void engine_test002a();
extern void engine_test002b();
extern void engine_test002c();
extern void engine_test003();
extern void engine_test004();
extern void engine_test005();
extern void engine_test006a();
extern void engine_test006b();
extern void engine_test006c();
extern void engine_test006d();
extern void engine_test007();
extern void engine_test008a();
extern void engine_test008b();
extern void engine_test008c();
extern void engine_test008d();
extern void engine_test008e();

extern void engine_test009a();
extern void engine_test009b();
extern void engine_test009c();
extern void engine_test010();
extern void engine_test011();
extern void engine_test012();

void create_test001( ){
  startTestSerie( " Creating a document from scratch. " );
  Document d( "xml:id='example'" );
  assertNoThrow( d.declare( AnnotationType::TOKEN,
			    "adhocset",
			    "annotator='proycon'" ) );
  assertEqual( d.default_set(AnnotationType::TOKEN), "adhocset" );
  assertEqual( d.default_annotator(AnnotationType::TOKEN), "proycon" );
  FoliaElement *text = 0;
  KWargs kw = getArgs( "xml:id""='" + d.id() + ".text.1'" );
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
  Document d( "xml:id""='example'" );
  assertNoThrow( d.declare( AnnotationType::POS,
				      "adhocset",
				      "annotator='proycon'" ) );
  assertNoThrow( d.declare( AnnotationType::POS,
				      "myset",
				      "annotator='sloot'" ) );
  assertEqual( d.default_set(AnnotationType::POS), "" );
  assertEqual( d.default_annotator(AnnotationType::POS), "" );
  assertEqual( d.default_annotator(AnnotationType::POS, "myset"), "sloot" );
  string id = d.id() + ".text.1";
  FoliaElement *text = 0;
  KWargs kw = getArgs( "xml:id""='" + id + "'" );
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
  Document d( "xml:id""='example'" );
  assertNoThrow( d.declare( AnnotationType::GAP,
				      "gap-set",
				      "annotator='sloot'" ) );
  assertNoThrow( d.declare( AnnotationType::GAP,
				      "extended-gap-set",
				      "annotator='sloot'" ) );
  assertEqual( d.default_set(AnnotationType::GAP), "" );
  assertEqual( d.default_annotator(AnnotationType::GAP), "" );
  assertEqual( d.default_annotator(AnnotationType::GAP, "gap-set"), "sloot" );
  assertEqual( d.default_annotator(AnnotationType::GAP, "extended-gap-set"), "sloot" );
  string id = d.id() + ".text.1";
  FoliaElement *text = 0;
  KWargs kw = getArgs( "xml:id""='" + id + "'" );
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
  Document d( "xml:id""='example'" );
  FoliaElement *text = 0;
  KWargs args;
  args["xml:id"] =  "t.1";
  assertNoThrow( text = d.addText( args ) );
  args["xml:id"] = "p.1";
  FoliaElement *p = 0;
  assertNoThrow( p = new Paragraph( args ) );
  text->append( p );
  args["xml:id"] = "s.1";
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
  Document d( "xml:id""='example'" );
  FoliaElement *text = 0;
  KWargs args;
  args["xml:id"] = "t.1";
  assertNoThrow( text = d.addText( args ) );
  args["xml:id"] = "p.1";
  FoliaElement *p = 0;
  assertNoThrow( p = new Paragraph( args ) );
  text->append( p );
  args["xml:id"] = "s.1";
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
  assertThrow( s->append( t3 ), DuplicateAnnotationError );
  s->addWord( "dit" );
  s->addWord( "is" );
  assertThrow( s->addWord( "geen" ), InconsistentText );
  s->addWord( "\t  tekst \n" );
}

void correction_test001a( ){
  startTestSerie( " Split correction " );
  Document corDoc( "xml:id""='example'" );
  assertNoThrow( corDoc.declare( AnnotationType::TOKEN,
				 "adhocset",
				 "annotator='proycon'" ) );
  Text *text = new Text( getArgs("xml:id""='" + corDoc.id() + ".text.1'") );
  assertNoThrow( corDoc.addText( text ) );
  FoliaElement *s = text->append( new Sentence( getArgs("xml:id""='" + corDoc.id() + ".s.1'") ) );
  s->append( new Word( getArgs("text='De', " "xml:id""='" + corDoc.id() + ".s.1.w.1'" ),
		       &corDoc ));
  s->append( new Word( getArgs("text='site', " "xml:id""='" + corDoc.id() + ".s.1.w.2'" ),
		       &corDoc ));
  s->append( new Word( getArgs("text='staat', " "xml:id""='" + corDoc.id() + ".s.1.w.3'" ),
		       &corDoc ));
  s->append( new Word( getArgs("text='online', " "xml:id""='" + corDoc.id() + ".s.1.w.4'" ),
		       &corDoc ));
  s->append( new Word( getArgs("text='.', " "xml:id""='" + corDoc.id() + ".s.1.w.5'" ),
		       &corDoc ));
  FoliaElement *w = corDoc.index(corDoc.id() + ".s.1.w.4");
  w->split( new Word( getArgs("xml:id""='" + corDoc.id() + ".s.1.w.4a', text='on'" ),
		      &corDoc ),
   	    new Word( getArgs("xml:id""='" + corDoc.id() + ".s.1.w.4b', text='line'" ),
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
    Document corDoc( "xml:id""='example'" );
  assertNoThrow( corDoc.declare( AnnotationType::TOKEN,
				 "adhocset",
				 "annotator='proycon'" ) );
  Text *text = new Text( getArgs( "xml:id""='" + corDoc.id() + ".text.1'") );
  assertNoThrow( corDoc.addText( text ) );
  FoliaElement *s = text->append( new Sentence( getArgs("xml:id""='" + corDoc.id() + ".s.1'"  ) ) );
  s->append( new Word( getArgs( "text='De', " "xml:id""='" + corDoc.id() + ".s.1.w.1'" ),
		       &corDoc ) );
  s->append( new Word( getArgs( "text='site', " "xml:id""='" + corDoc.id() + ".s.1.w.2'" ),
		       &corDoc ) );
  s->append( new Word( getArgs("text='staat', " "xml:id""='" + corDoc.id() + ".s.1.w.3'" ),
		       &corDoc ));
  s->append( new Word( getArgs( "text='online', " "xml:id""='" + corDoc.id() + ".s.1.w.4'" ),
		       &corDoc ));
  s->append( new Word( getArgs( "text='.', " "xml:id""='" + corDoc.id() + ".s.1.w.5'" ),
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
  assertEqual( s->xmlstring(), "<s xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example.s.1\"><w xml:id=\"example.s.1.w.1\"><t>De</t></w><w xml:id=\"example.s.1.w.2\"><t>site</t></w><w xml:id=\"example.s.1.w.3\"><t>staat</t></w><correction xml:id=\"example.s.1.correction.1\"><current><w xml:id=\"example.s.1.w.4\"><t>online</t></w></current><suggestion auth=\"no\"><w xml:id=\"example.s.1.w.6\"><t>on</t></w><w xml:id=\"example.s.1.w.7\"><t>line</t></w></suggestion></correction><w xml:id=\"example.s.1.w.5\"><t>.</t></w></s>" );
}

void correction_test002(){
  startTestSerie( " Merge corrections " );
  Document corDoc( "xml:id""='example'" );
  assertNoThrow( corDoc.declare( AnnotationType::TOKEN,
				  "adhocset",
				  "annotator='proycon'" ) );
  Text *text = new Text( getArgs("xml:id""='" + corDoc.id() + ".text.1'") );
  assertNoThrow( corDoc.addText( text ) );
  FoliaElement *s = text->append( new Sentence( getArgs("xml:id""='" + corDoc.id() + ".s.1'"	) ) );
  s->append( new Word( getArgs( "text='De', " "xml:id""='" + corDoc.id() + ".s.1.w.1'" ),
		       &corDoc ) );
  s->append( new Word( getArgs( "text='site', " "xml:id""='" + corDoc.id() + ".s.1.w.2'" ),
		       &corDoc ) );
  s->append( new Word( getArgs( "text='staat', " "xml:id""='" + corDoc.id() + ".s.1.w.3'" ),
		       &corDoc ) );
  s->append( new Word( getArgs( "text='on', " "xml:id""='" + corDoc.id() + ".s.1.w.4'" ),
		       &corDoc ) );
  s->append( new Word( getArgs( "text='line', " "xml:id""='" + corDoc.id() + ".s.1.w.5'" ),
		       &corDoc ) );
  s->append( new Word( getArgs( "text='.', " "xml:id""='" + corDoc.id() + ".s.1.w.6'" ),
		       &corDoc ) );

  vector<FoliaElement *> ow;
  ow.push_back( corDoc.index(corDoc.id() + ".s.1.w.4") );
  ow.push_back( corDoc.index(corDoc.id() + ".s.1.w.5") );
  s->mergewords( new Word( getArgs("xml:id""='" + corDoc.id() + ".s.1.w.4-5', text='online'"),
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
  Document corDoc( "xml:id""='example'" );
  assertNoThrow( corDoc.declare( AnnotationType::TOKEN,
				  "adhocset",
				  "annotator='proycon'" ) );
  Text *text = new Text( getArgs("xml:id""='" + corDoc.id() + ".text.1'") );
  assertNoThrow( corDoc.addText( text ) );
  FoliaElement *s = text->append( new Sentence( getArgs("xml:id""='" + corDoc.id() + ".s.1'"	) ) );
  s->append( new Word( getArgs( "text='Ik', " "xml:id""='" + corDoc.id() + ".s.1.w.1'"),
		       &corDoc ) );
  s->append( new Word( getArgs( "text='zie', " "xml:id""='" + corDoc.id() + ".s.1.w.2'" ),
		       &corDoc ));
  s->append( new Word( getArgs( "text='een', " "xml:id""='" + corDoc.id() + ".s.1.w.3'" ),
		       &corDoc ));
  s->append( new Word( getArgs("text='groot', " "xml:id""='" + corDoc.id() + ".s.1.w.4'" ),
		       &corDoc ));
  s->append( new Word( getArgs("text='huis', " "xml:id""='" + corDoc.id() + ".s.1.w.5'" ),
		       &corDoc ));
  s->append( new Word( getArgs("text='.', " "xml:id""='" + corDoc.id() + ".s.1.w.6'" ),
		       &corDoc ));

  s->deleteword( corDoc.index( corDoc.id() + ".s.1.w.4" ) );
  assertNoThrow( corDoc.save( "/tmp/foliadelete003.xml" ) );
  assertEqual( s->text(), "Ik zie een huis ." );
  assertEqual( s->xmlstring(), "<s xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example.s.1\"><w xml:id=\"example.s.1.w.1\"><t>Ik</t></w><w xml:id=\"example.s.1.w.2\"><t>zie</t></w><w xml:id=\"example.s.1.w.3\"><t>een</t></w><correction xml:id=\"example.s.1.correction.1\"><new/><original auth=\"no\"><w xml:id=\"example.s.1.w.4\"><t>groot</t></w></original></correction><w xml:id=\"example.s.1.w.5\"><t>huis</t></w><w xml:id=\"example.s.1.w.6\"><t>.</t></w></s>");
}

void correction_test004(){
  startTestSerie( " Insert corrections " );
  Document corDoc( "xml:id""='example'" );
  assertNoThrow( corDoc.declare( AnnotationType::TOKEN,
				  "adhocset",
				  "annotator='proycon'" ) );
  Text *text = new Text( getArgs("xml:id""='" + corDoc.id() + ".text.1'") );
  assertNoThrow( corDoc.addText( text ) );
  FoliaElement *s = text->append( new Sentence( getArgs("xml:id""='" + corDoc.id() + ".s.1'"	) ) );
  s->append( new Word( getArgs("text='Ik', " "xml:id""='" + corDoc.id() + ".s.1.w.1'" ),
		       &corDoc ));
  s->append( new Word( getArgs("text='zie', " "xml:id""='" + corDoc.id() + ".s.1.w.2'"),
		       &corDoc ));
  s->append( new Word( getArgs( "text='een', " "xml:id""='" + corDoc.id() + ".s.1.w.3'" ),
		       &corDoc ));
  s->append( new Word( getArgs("text='huis', " "xml:id""='" + corDoc.id() + ".s.1.w.4'" ),
		       &corDoc ));
  s->append( new Word( getArgs("text='.', " "xml:id""='" + corDoc.id() + ".s.1.w.5'" ),
		       &corDoc ));

  Word *w = new Word( getArgs( "xml:id""='" + corDoc.id() + ".s.1.w.3b', text='groot'"), &corDoc );
  FoliaElement *w2 = 0;
  assertNoThrow( w2 = corDoc.index( corDoc.id() + ".s.1.w.3" ) );
  assertNoThrow( s->insertword( w, w2 ) );

  //  assertNoThrow( corDoc.save( "/tmp/foliainsert004.xml" ) );
  assertEqual( s->words().size(), 6 );
  assertEqual( s->text(), "Ik zie een groot huis ." );
  assertEqual( s->xmlstring(), "<s xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example.s.1\"><w xml:id=\"example.s.1.w.1\"><t>Ik</t></w><w xml:id=\"example.s.1.w.2\"><t>zie</t></w><w xml:id=\"example.s.1.w.3\"><t>een</t></w><correction xml:id=\"example.s.1.correction.1\"><new><w xml:id=\"example.s.1.w.3b\"><t>groot</t></w></new></correction><w xml:id=\"example.s.1.w.4\"><t>huis</t></w><w xml:id=\"example.s.1.w.5\"><t>.</t></w></s>" );
}

void correction_test005(){
  startTestSerie( " Re-using a correction with only suggestions " );
  Document *corDoc = new Document();
  corDoc->read_from_file( "tests/example.xml" );
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
  assertEqual( w->annotation<Correction>()->getNew()->text(), "stippellijn" );
  assertEqual( w->annotation<Correction>()->annotator(), "John Doe" );
  assertEqual( w->annotation<Correction>()->annotatortype(), MANUAL );

  assertEqual( w->xmlstring(), "<w xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.11\"><pos class=\"FOUTN(soort,ev,basis,zijd,stan)\" set=\"https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn\"/><lemma class=\"stippelijn\"/><correction xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.11.correction.1\" annotator=\"John Doe\" class=\"spelling\"><suggestion auth=\"no\"><t>stippellijn</t></suggestion><new><t>stippellijn</t></new><original auth=\"no\"><t>stippelijn</t></original></correction></w>" );
  delete corDoc;
}

void correction_test006(){
  startTestSerie( "Correction - Suggestion for deletion with parent merge suggestion" );
  Document *corDoc = new Document();
  corDoc->read_from_file( "tests/example.xml" );
  KWargs args;
  args["xml:id"] = corDoc->id() + ".s.1";
  Sentence *sent = new Sentence( args );
  FoliaElement *root = corDoc->doc();
  FoliaElement *text =0;
  assertNoThrow( text = root->index(0) );
  assertNoThrow( text->append( sent ) );
  args["xml:id"] = corDoc->id() + ".s.1.w.1";
  args["text"] = "De";
  Word *wrd = new Word( args );
  assertNoThrow( sent->append( wrd ) );
  args["xml:id"] = corDoc->id() + ".s.1.w.2";
  args["text"] = "site";
  wrd = new Word( args );
  assertNoThrow( sent->append( wrd ) );
  args["xml:id"] = corDoc->id() + ".s.1.w.3";
  args["text"] = "staat";
  wrd = new Word( args );
  assertNoThrow( sent->append( wrd ) );
  args["xml:id"] = corDoc->id() + ".s.1.w.4";
  args["text"] = "on";
  wrd = new Word( args );
  assertNoThrow( sent->append( wrd ) );
  args["xml:id"] = corDoc->id() + ".s.1.w.5";
  args["text"] = "line";
  wrd = new Word( args );
  assertNoThrow( sent->append( wrd ) );
  args["xml:id"] = corDoc->id() + ".s.1.w.6";
  args["text"] = ".";
  wrd = new Word( args );
  assertNoThrow( sent->append( wrd ) );
  args.clear();
  args["xml:id"] = corDoc->id() + ".s.2";
  sent = new Sentence( args);
  text->append( sent );
  args["xml:id"] = corDoc->id() + ".s.2.w.1";
  args["text"] = "sinds";
  wrd = new Word( args );
  assertNoThrow( sent->append( wrd ) );
  args["xml:id"] = corDoc->id() + ".s.2.w.2";
  args["text"] = "vorige";
  wrd = new Word( args );
  assertNoThrow( sent->append( wrd ) );
  args["xml:id"] = corDoc->id() + ".s.2.w.3";
  args["text"] = "week";
  wrd = new Word( args );
  assertNoThrow( sent->append( wrd ) );
  args["xml:id"] = corDoc->id() + ".s.2.w.4";
  args["text"] = "zondag";
  wrd = new Word( args );
  assertNoThrow( sent->append( wrd ) );
  args["xml:id"] = corDoc->id() + ".s.2.w.5";
  args["text"] = ".";
  wrd = new Word( args );
  assertNoThrow( sent->append( wrd ) );

  FoliaElement *s = corDoc->index(corDoc->id() + ".s.1");
  FoliaElement *s2 = corDoc->index(corDoc->id() + ".s.2");
  FoliaElement *w = corDoc->index(corDoc->id() + ".s.1.w.6");
  s->remove(w); // doesn't delete w, we gonna re-use it
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
  corDoc.read_from_file( "tests/corr_str.xml" );
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

void correction_test008a(){
  startTestSerie( " Correction - correct a correction" );
  Document cor_doc( "tests/example.xml" );
  FoliaElement *w = cor_doc["WR-P-E-J-0000000001.p.1.s.6.w.31.c.1"]; // a corrected word
  // and try to correct it again
  KWargs args;
  args["value"] = "ronde";
  TextContent *nt = new TextContent( args );
  vector<FoliaElement*> nv;
  nv.push_back( nt );
  vector<FoliaElement*> ov;
  ov.push_back( w->getNew() );
  vector<FoliaElement*> cv;
  vector<FoliaElement*> sv;
  args = getArgs( "set='corrections',annotator='testscript', annotatortype='MANUAL'" );
  FoliaElement *cor = w->correct( ov, cv, nv, sv, args );
  assertEqual( cor->text(), "ronde" );
  assertEqual( cor->xmlstring(),
	       "<correction xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.6.w.31.correction.1\" annotator=\"testscript\"><new><t>ronde</t></new><original auth=\"no\"><correction xml:id=\"WR-P-E-J-0000000001.p.1.s.6.w.31.c.1\"><new><t>vierkante</t></new><original auth=\"no\"><t>vierkant</t></original></correction></original></correction>" );
}

void correction_test008b(){
  startTestSerie( " Correction - correct a correction (alternative)" );
  Document cor_doc( "tests/example.xml" );
  FoliaElement *w = cor_doc["WR-P-E-J-0000000001.p.1.s.6.w.31.c.1"]; // a corrected word
  FoliaElement *new_node = w->getNew();
  // and try to correct it again
  string args = "new='ronde', set='corrections',annotator='testscript', annotatortype='MANUAL'";
  FoliaElement *cor = new_node->correct( args );
  assertEqual( cor->text(), "ronde" );
  assertEqual( cor->xmlstring(),
	       "<correction xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.6.w.31.correction.1\" annotator=\"testscript\"><new><t>ronde</t></new><original auth=\"no\"><correction xml:id=\"WR-P-E-J-0000000001.p.1.s.6.w.31.c.1\"><new><t>vierkante</t></new><original auth=\"no\"><t>vierkant</t></original></correction></original></correction>" );
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
  Document doc( "xml:id""='test'" );
  Text *text = new Text( getArgs("xml:id""='test.text'") );
  doc.addText( text );
  FoliaElement *s = new Sentence( getArgs( "xml:id""='" + doc.id() + ".s.1'") );
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
  Document doc( "xml:id""='test'" );
  Text *text = new Text( getArgs("xml:id""='test.text'") );
  doc.addText( text );
  FoliaElement *s = new Sentence( getArgs("xml:id""='" + doc.id() + ".s.1'") );
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

Document whitespaceDoc( "file='" + fol_path + "examples/tests/issue88b.2.5.1.folia.xml'" );

void whitespace_test001(){
  startTestSerie(" Whitespace - Heavy markuped up, double nested and multiline " );
  FoliaElement *p = whitespaceDoc["test.p.1"];
  assertTrue( p->text() == "deel woord extra" );
}

void whitespace_test002(){
  startTestSerie(" Whitespace - Multiline markup " );
  FoliaElement *p = whitespaceDoc["test.p.2"];
  assertTrue( p->text() == "deel woord extra" );
}

void whitespace_test003(){
  startTestSerie(" Whitespace - Multiline plain " );
  FoliaElement *p = whitespaceDoc["test.p.3"];
  assertTrue( p->text() == "deel woord extra" );
}

void whitespace_test004(){
  startTestSerie(" Whitespace - Single line simple markup " );
  FoliaElement *p = whitespaceDoc["test.p.4"];
  assertTrue( p->text() == "deel woord extra" );
}

void whitespace_test005(){
  startTestSerie(" Whitespace - Strip leading/trailing space " );
  FoliaElement *p = whitespaceDoc["test.p.5"];
  assertTrue( p->text() == "deel woord extra" );
}

void whitespace_test006(){
  startTestSerie(" Whitespace - Collapse multi-space " );
  FoliaElement *p = whitespaceDoc["test.p.6"];
  assertTrue( p->text() == "deel woord extra" );
}

void whitespace_test007(){
  startTestSerie(" Whitespace - Preserve multi-space " );
  FoliaElement *p = whitespaceDoc["test.p.7"];
  assertTrue( p->text() == "deel  woord  extra" );
}

void whitespace_test008(){
  startTestSerie(" Whitespace - Preserve leading/trailing space " );
  FoliaElement *p = whitespaceDoc["test.p.8"];
  assertTrue( p->text() == "  deel      woord        extra" );
}


void whitespace_test009(){
  startTestSerie(" Whitespace - No space between non-spaced markup elements " );
  FoliaElement *p = whitespaceDoc["test.p.9"];
  assertTrue( p->text() == "deelwoord extra" );
}

void whitespace_test010(){
  startTestSerie(" Whitespace - Explicit linebreaks " );
  FoliaElement *p = whitespaceDoc["test.p.10"];
  assertTrue( p->text() == "deel\nwoord\nextra\n" );
}

void whitespace_test010b(){
  startTestSerie(" Whitespace - Explicit linebreaks " );
  FoliaElement *p = whitespaceDoc["test.p.10b"];
  assertTrue( p->text() == "deel\nwoord\nextra\n" );
}

void whitespace_test011(){
  startTestSerie(" Whitespace - Running text " );
  FoliaElement *p = whitespaceDoc["test.p.11"];
  assertTrue( p->text() == "I Buiten- en binnenlandse hoogleraren, lectoren en oud-docenten in de neerlandistiek, sprekers, bestuurs- en stafleden van de IVN." );
}
void whitespace_test012(){
  startTestSerie(" Whitespace - Running text with space after markup " );
  FoliaElement *p = whitespaceDoc["test.p.12"];
  assertTrue( p->text() == "I Buiten- en binnenlandse hoogleraren, lectoren en oud-docenten in de neerlandistiek, sprekers, bestuurs- en stafleden van de IVN ." );
}

void whitespace_test013(){
  startTestSerie(" Whitespace - No space despite t-hbr, markup and features " );
  FoliaElement *p = whitespaceDoc["test.p.13"];
  assertTrue( p->text() == "Es entspricht einerseits nicht den Erwartungen derjenigen, welche in betreff der Lage der Landarbeiter nur solche" );
}

void whitespace_test014(){
  startTestSerie(" Whitespace - Maintain trailing space " );
  FoliaElement *p = whitespaceDoc["test.p.14"];
  assertTrue( p->text() == "Terwijl hij den gemiddelden prijs van een karaat geslepen op £ 8 stelde, den prijs van een steen van 2 karaat op 22 X 8 = £ 32" );
}


void usage(){
  cerr << "usage: foliatest [options]" << endl;
  cerr << "options are" << endl;
  cerr << "\t-h or --help\t\t This help" << endl;
  cerr << "\t-V or --version\t\t Show versions" << endl;
  cerr << "\t--dir\t\t\t Extra directory to lookup example folia files." << endl;
  cerr << "\t-e or --expect set this value to hint the number of errors to expect." << endl;
}

int main( int argc, char* argv[] ){
  try {
    TiCC::CL_Options Opts( "hVe:",
			   "expect:,help,version,dir:");
    Opts.init(argc, argv );
    if ( Opts.extract( 'h' )
	 || Opts.extract( "help" ) ){
      usage();
      return EXIT_SUCCESS;
    }
    if ( Opts.extract( 'V' )
	 || Opts.extract( "version" ) ){
      cout << "foliatest for " << folia::VersionName() << endl;
      cout << "build to test version: " << FOLIA_INT_VERSION << endl;
      return EXIT_SUCCESS;
    }
    string value;
    if ( Opts.extract( 'e', value )
	 || Opts.extract( "expect", value ) ){
      if ( !TiCC::stringTo( value, expect ) ){
	cerr << "illegal value '" << value << "' for --expect/-e option."
	     << endl;
	return EXIT_FAILURE;
      }
    }
    Opts.extract( "dir", default_path );
  }
  catch ( const exception& e ){
    cerr << e.what() << endl;
    return EXIT_FAILURE;
  }
  bool is_setup = setup();
  //  exit(777);
  test0();
  test1();
  test1a();
  test1b();
  test1c();
  test1d();
  test1e();
  test1f();
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
  sanity_test100();
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
  sanity_test130();
  sanity_test140();
  sanity_test141();
  sanity_test150();
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
  edit_test019();
  text_test01();
  text_test03();
  text_test04();
  text_test05();
  text_test06();
  text_test07();
  text_test08();
  text_test08b();
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
  text_test18();
  text_test19a();
  text_test19b();
  text_test20();
  create_test001();
  create_test002();
  create_test003();
  create_test004();
  create_test005();
  correction_test001a();
  correction_test001b();
  correction_test002();
  correction_test003();
  correction_test004();
  correction_test005();
  correction_test006();
  correction_test007();
  correction_test008a();
  correction_test008b();
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
  engine_test001a();
  engine_test001b();
  engine_test001c();
  engine_test001d();
  engine_test001e();
  engine_test001f();
  engine_test002a();
  engine_test002b();
  engine_test002c();
  engine_test003();
  engine_test004();
  engine_test005();
  engine_test006a();
  engine_test006b();
  engine_test006c();
  engine_test006d();
  engine_test007();
  engine_test008a();
  engine_test008b();
  engine_test008c();
  engine_test008d();
  engine_test008e();
  engine_test009a();
  engine_test009b();
  engine_test009c();
  engine_test010();
  engine_test011();
  whitespace_test001();
  whitespace_test002();
  whitespace_test003();
  whitespace_test004();
  whitespace_test005();
  whitespace_test006();
  whitespace_test007();
  whitespace_test008();
  whitespace_test009();
  whitespace_test010();
  whitespace_test011();
  whitespace_test012();
  whitespace_test013();
  whitespace_test014();
  if ( !is_setup ){
    assertMessage( "FOLIAPATH not set?", false );
  }
  else {
    engine_test012();
    Test_E001_Tokens_Structure();
    Test_Exxx_Hidden_Tokens();
    Test_Exxx_Invalid_Wref();
    Test_Exxx_KeepVersion();
    Test_Exxx_SetAndSetLess();
    Test_Provenance();
  }
  summarize_tests(expect);
}
