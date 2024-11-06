/*
  Copyright (c) 2006 - 2024
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
extern string default_path;
extern string legacy_file;
extern Document LEGACYEXAMPLE;
extern string fol_path;

void Test_E001_Tokens_Structure(){
  Document doc( fol_path + "examples/tokens-structure.2.0.0.folia.xml" );
  vector<Word*> words = doc.words();
  vector<Sentence*> sentences = doc.sentences();
  vector<Paragraph*> paragraphs = doc.paragraphs();
  {
    startTestSerie( " Simple Token - Word count " );
    assertEqual( words.size(), 8 );
  }
  {
    startTestSerie( " Simple Token - Word ID's " );
    static vector<string> cmp( {"example.p.1.s.1.w.1", "example.p.1.s.1.w.2",
	  "example.p.1.s.1.w.3", "example.p.1.s.2.w.1", "example.p.1.s.2.w.2",
	  "example.p.1.s.2.w.3", "example.p.1.s.2.w.4", "example.p.1.s.2.w.5"});
    int i = 0;
    for( auto const& w : words ){
      assertEqual( w->id(), cmp[i++] );
    }
  }
  {
    startTestSerie( " Simple Token - Structure Count " );
    assertEqual( sentences.size(), 2 );
    assertEqual( paragraphs.size(), 1 );
  }
  {
    startTestSerie( " Simple Token - Structure ID's " );
    static vector<string> cmp_s( {"example.p.1.s.1", "example.p.1.s.2"} );
    int i = 0;
    for( auto const& s : sentences ){
      assertEqual( s->id(), cmp_s[i++] );
    }
    static vector<string> cmp_p( {"example.p.1"} );
    i = 0;
    for( auto const& p : paragraphs ){
      assertEqual( p->id(), cmp_p[i++] );
    }
  }
  {
    startTestSerie("Simple Token & Structure Test - First word");
    FoliaElement *w = words[0];
    assertTrue( w->isinstance( Word_t ) );
    assertEqual( w->id(), "example.p.1.s.1.w.1" );
    assertEqual( w->text(), "Hello" );
    assertEqual( str(w), "Hello" );
  }
  {
    startTestSerie("Simple Token & Structure Test - last word");
    FoliaElement *w = doc.rwords(0);
    assertTrue( w->isinstance<Word>() );
    assertEqual( w->id(), "example.p.1.s.2.w.5" );
    assertEqual( w->text(), "." );
    assertEqual( str(w), "." );
  }
  {
    startTestSerie( "Simple Token & Structure Test - Sentence" );
    //grab second sentence
    FoliaElement *s = doc.sentences(1);
    assertTrue( isinstance( s, Sentence_t) );
    assertEqual( s->id(), "example.p.1.s.2" );
    assertFalse( s->hastext() ); //no explicit text
    assertEqual( str(s), "This is an example." );
  }
  {
    startTestSerie( "Simple Token & Structure Test - Index" );
    // rab something using the index
    FoliaElement *w = doc["example.p.1.s.1.w.1"];
    assertTrue( isinstance( w, Word_t ) );
    assertEqual( doc["example.p.1.s.1.w.1"],
		 doc.index("example.p.1.s.1.w.1") );
    assertEqual( w->id(), "example.p.1.s.1.w.1" );
    assertEqual( w->text(), "Hello" )
  }
  {
    startTestSerie( "Simple Token & Structure Test - Declarations" );
    assertTrue( doc.declared(AnnotationType::TOKEN) );
    assertTrue( doc.declared(Word_t) ); // same as above, resolves automatically
    assertTrue( doc.declared(AnnotationType::TEXT) );
    assertTrue( doc.declared(TextContent_t) ); //same as above, resolves automatically
    assertTrue( doc.declared(Sentence_t) );
    assertTrue( doc.declared(Paragraph_t) );
  }
}

void Test_Exxx_Hidden_Tokens(){ // xxx -> replace with a number at some point when there are more new tests
  //    """Hidden token tests"""
  {
    Document doc( fol_path + "examples/hiddentokens.2.0.0.folia.xml" );

    {
      startTestSerie( "Simple Token & Structure - Word count (does not include hidden words)" );
      vector<Word*> wv = doc.words();
      assertEqual( wv.size(), 7 );
      FoliaElement *sentence = doc["example.s.1"];
      assertEqual( sentence->words().size(), 7 );
      wv = sentence->select<Word>( default_ignore_structure );
      assertEqual( wv.size(), 7 );
    }

    {
      startTestSerie( "Text serialisation on sentence (no hidden words)" );
      FoliaElement *sentence = doc["example.s.1"];
      assertEqual( sentence->text() , "Isn't a whole lot left." );
    }

    {
      startTestSerie( "Text serialisation on syntactic unit (no hidden words)" );
      FoliaElement *su = doc["example.s.1.su.1"];
      assertEqual( su->text() , "Isn't a whole lot left." );
    }

    {
      startTestSerie( "Text serialisation on the hidden word itself" );
      FoliaElement *hw = doc["example.s.1.w.0"];
      assertEqual( hw->text(TEXT_FLAGS::HIDDEN) , "*exp*" );
    }

    {
      startTestSerie( "Check whether hidden word is part of the wrefs of Synatactic Unit" );
      static vector<string> wref_ids( {"example.s.1.w.0", "example.s.1.w.1",
	    "example.s.1.w.2", "example.s.1.w.3", "example.s.1.w.4",
	    "example.s.1.w.5", "example.s.1.w.6", "example.s.1.w.7"});
      FoliaElement *su = doc["example.s.1.su.1"];
      vector<FoliaElement*> wrefs = su->wrefs();
      int i = 0;
      for( auto const& w : wrefs ){
	assertEqual( w->id(), wref_ids[i++] );
      }
    }

    {
      startTestSerie( "Text serialisation on syntactic unit (with hidden word)" );
      FoliaElement *su = doc["example.s.1.su.1"];
      assertEqual( su->text(TEXT_FLAGS::HIDDEN) , "*exp* Isn't a whole lot left." );
    }

    {
      startTestSerie( "Test XML serialisation of wrefs in syntactic unit (with hidden words" );
      FoliaElement *su = doc["example.s.1.su.1"];
      string value = su->xmlstring();
      string cmp = "<su xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example.s.1.su.1\" class=\"IP-MAT\"><su xml:id=\"example.s.1.su.2\" class=\"NP-SBJ\"><wref id=\"example.s.1.w.0\"/></su><su xml:id=\"example.s.1.su.3\" class=\"VP\"><su xml:id=\"example.s.1.su.4\" class=\"BEP\"><wref id=\"example.s.1.w.1\" t=\"Is\"/></su><su xml:id=\"example.s.1.su.5\" class=\"NEG\"><wref id=\"example.s.1.w.2\" t=\"n't\"/></su><su xml:id=\"example.s.1.su.6\" class=\"VP\"><su xml:id=\"example.s.1.su.7\" class=\"NP-LGS\"><wref id=\"example.s.1.w.3\" t=\"a\"/><su xml:id=\"example.s.1.su.8\" class=\"ADJP\"><wref id=\"example.s.1.w.4\" t=\"whole\"/></su><wref id=\"example.s.1.w.5\" t=\"lot\"/></su><wref id=\"example.s.1.w.6\" t=\"left\"/></su></su><su class=\"PUNC\"><wref id=\"example.s.1.w.7\" t=\".\"/></su></su>";
      assertEqual( value, cmp );
    }
  }
}

void Test_Exxx_Invalid_Wref(){ // xxx -> replace with a number at some point
  startTestSerie( "Invalid Wref test" );
  Document *doc = 0;
  assertThrow( doc = new Document( "file='" + fol_path + "examples/erroneous/invalid-wref.2.0.0.folia.xml'" ), XmlError );
  assertTrue( doc == 0 );
  if ( doc ){
    // useless code to keep scan-build happy
    delete doc;
  }
}

void Test_Exxx_KeepVersion(){ // xxx -> replace with a number at some point
  startTestSerie( "Serialization of older FoLiA versions" );
  assertNoThrow( LEGACYEXAMPLE.save( "/tmp/example.xml" ) );
  string cmd = "./tests/foliadiff.sh /tmp/example.xml " + legacy_file;
  int stat = system( cmd.c_str() );
  assertMessage( "/tmp/example.xml " + legacy_file + " differ!",
   		 (stat == 0) );
}

void Test_Exxx_SetAndSetLess(){ // xxx -> replace with a number at some point
  Document sdoc( fol_path + "examples/tests/set_and_setless.2.0.0.folia.xml" );
  {
    startTestSerie( "Testing sanity with set-holding and setless annotation types similtaneously (setless)" );
    auto c1 = sdoc["example.p.1.s.1.chunk.1"];
    assertEqual( c1->sett(), "" );
  }
  {
    startTestSerie( "Testing sanity with set-holding and setless annotation types similtaneously (set-holding)" );
    auto c1 = sdoc["example.p.1.s.1.chunkset.1"];
    assertEqual( c1->sett(), "chunkset" );
  }
  {
    Document doc( fol_path + "examples/tests/set_holding.2.0.0.folia.xml" );
    startTestSerie( "Testing sanity with adding setless to setholding chunks" );
    assertEqual( doc.default_set(AnnotationType::CHUNKING), "chunkset" );
    assertEqual( doc.default_processor(AnnotationType::CHUNKING), "p1" );
    KWargs args;
    args["name"] = "my_proc";
    args["xml:id"] = "my_chunker";
    doc.add_processor( args );
    args.clear();
    args["processor"] = "my_chunker";
    doc.declare( AnnotationType::CHUNKING, "", args );
    // now there are 2 chunkers active, so NO default set anymore
    assertEqual( doc.default_set(AnnotationType::CHUNKING), "" );
    // now there are 2 chunkers active, and NO default processor too
    assertEqual( doc.default_processor(AnnotationType::CHUNKING), "" );
    auto s = doc["example.p.1.s.1"];
    args.clear();
    ChunkingLayer *c = 0;
    assertNoThrow( c = new ChunkingLayer( args, &doc ) );
    s->append( c );
    Chunk *ch = 0;
    args["class"] = "NP";
    args["processor"] = "my_chunker";
    assertNoThrow( ch = new Chunk( args, &doc ) );
    c->append( ch );
    ch->append( doc["example.p.1.s.1.w.1"] );
    ch->append( doc["example.p.1.s.1.w.2"] );
    ch->append( doc["example.p.1.s.1.w.3"] );
    assertNoThrow( doc.save( "/tmp/set_test.xml" ) );
    string cmd = "./tests/foliadiff.sh /tmp/set_test.xml tests/set_test.ok";
    int stat = system( cmd.c_str() );
    assertMessage( "/tmp/set_test.xml tests/set_test.ok differ!",
		   (stat == 0) );
  }
}

bool xmldiff( const string& f1, const string& f2 ){
  string cmd = "./tests/foliadiff.sh " + f1 + " " + f2;
  int stat = system( cmd.c_str() );
  return stat == 0;
}

void Test_Provenance(){
  {
    Document doc( fol_path + "examples/provenance.2.0.0.folia.xml" );
    {
      startTestSerie( "Provenance - Parse and sanity check" );
      Provenance *provenance = doc.provenance();
      assertEqual((*provenance)["p0"]->name(), "ucto" );
      assertEqual((*provenance)["p0.1"]->name(), "libfolia");
      assertEqual((*provenance)["p1"]->name(), "frog");
#if FOLIA_INT_VERSION < 221
      assertEqual((*provenance)["p1"]->type(), AUTO );
#else
      assertEqual((*provenance)["p1"]->type(), AnnotatorType::AUTO );
#endif
      assertEqual(provenance->index("p1")->version(), "0.16");
      assertEqual((*provenance)["p1.0"]->name(), "libfolia");
#if FOLIA_INT_VERSION < 221
      assertEqual((*provenance)["p1.0"]->type(), GENERATOR );
#else
      assertEqual((*provenance)["p1.0"]->type(), AnnotatorType::GENERATOR );
#endif
      assertEqual((*provenance)["p1.0"]->name(), "libfolia");
      assertEqual((*provenance)["p2.1"]->name(), "proycon");
#if FOLIA_INT_VERSION < 221
      assertEqual((*provenance)["p2.1"]->type(), MANUAL );
#else
      assertEqual((*provenance)["p2.1"]->type(), AnnotatorType::MANUAL );
#endif
      auto annotators = doc.get_annotators( AnnotationType::POS,
					    "http://ilk.uvt.nl/folia/sets/frog-mbpos-cgn" );
      assertEqual(len(annotators), 3 );
      // basically the same thing as above, but resolved to Processor instances:
      auto processors = doc.get_processors( AnnotationType::POS,
					    "http://ilk.uvt.nl/folia/sets/frog-mbpos-cgn" );
      assertEqual( len(processors), 3 );
      // let's see if we got the right ones:
      assertEqual( processors[0]->id(), "p1.1" );
      assertEqual( processors[0]->name(), "mbpos" );
#if FOLIA_INT_VERSION < 221
      assertEqual( processors[0]->type(), AUTO );
#else
      assertEqual( processors[0]->type(), AnnotatorType::AUTO );
#endif
      assertEqual( processors[1]->name(), "proycon");
#if FOLIA_INT_VERSION < 221
      assertEqual( processors[1]->type(), MANUAL );
#else
      assertEqual( processors[1]->type(), AnnotatorType::MANUAL );
#endif
    }
    {
      startTestSerie( "Provenance - Annotation sanity check" );
      auto word = doc["untitled.p.1.s.1.w.1"];
      string pid = word->annotation<PosAnnotation>()->processor();
      assertEqual( pid, "p1.1" );
      auto proc = doc.get_processor( pid );
      assertEqual( proc->id(), "p1.1" );
      assertEqual( proc->name(), "mbpos" );
#if FOLIA_INT_VERSION < 221
      assertEqual( proc->type(), AUTO );
#else
      assertEqual( proc->type(), AnnotatorType::AUTO );
#endif
      // The old annotator attribute can also still be used and refers to the
      // processor name (for backward API compatibility)
      assertEqual( proc->annotator(), "mbpos" );
      // The old annotatortype attribute can also still be used and refers
      // to the processor type:
#if FOLIA_INT_VERSION < 221
      assertEqual( proc->annotatortype(), AUTO );
#else
      assertEqual( proc->annotatortype(), AnnotatorType::AUTO );
#endif
      word = doc["untitled.p.1.s.1.w.2"];
      pid = word->annotation<PosAnnotation>()->processor();
      assertEqual( pid, "p2.1" );
      proc = doc.get_processor( pid );
      assertEqual( proc->id(), "p2.1" );
      assertEqual( proc->name(), "proycon" );
#if FOLIA_INT_VERSION < 221
      assertEqual( proc->type(), MANUAL );
#else
      assertEqual( proc->type(), AnnotatorType::MANUAL );
#endif
    }
    {
      startTestSerie("Provenance - Checking default/implicit processor/annotator" );
      auto word = doc["untitled.p.1.s.1.w.2"];
      string pid = word->annotation<LemmaAnnotation>()->processor();
      auto proc = doc.get_processor( pid );
      assertEqual( proc->id(), "p1.2" );
      assertEqual( proc->name(), "mblem" );
#if FOLIA_INT_VERSION < 221
      assertEqual( proc->type(), AUTO );
#else
      assertEqual( proc->type(), AnnotatorType::AUTO );
#endif
      // The old annotator attribute can also still be used and refers to
      // the processor name
      assertEqual( proc->annotator(), "mblem");
    }

    {
      startTestSerie("Provenance - Create a document with a processor" );
      Document test("xml:id='test'");
      KWargs args;
      args["name"] = "TestSuite";
      args["xml:id"] = "p0";
      test.add_processor( args );
      assertEqual( test.provenance()->index("p0")->name(), "TestSuite");
      test.save( "/tmp/test-1.xml" );
      string xmlref =
	"<FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\" xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"test\" version=\"2.0.0\" generator=\"foliapy-v2.0.0\">\n"
	"  <metadata type=\"native\">\n"
	"    <annotations/>\n"
	"    <provenance>\n"
	"      <processor xml:id=\"p0\" name=\"TestSuite\" type=\"auto\"/>\n"
	"    </provenance>\n"
	"  </metadata>\n"
	"</FoLiA>\n";
      Document ref("string='" + xmlref + "'" );
      ref.save( "/tmp/ref-1.xml" );
      assertTrue( xmldiff( "/tmp/test-1.xml", "/tmp/ref-1.xml" ) );
    }


    {
      startTestSerie("Provenance - provenance: Duplicate named and ID's" );
      Document test("xml:id='test'");
      KWargs args;
      args["name"] = "TestSuite";
      args["xml:id"] = "p0";
      test.add_processor( args );
      args.clear();
      args["name"] = "TestSuite"; // duplicate names is OK!
      args["xml:id"] = "p1";
      test.add_processor( args );
      vector<processor *> pv = test.get_processors_by_name( "TestSuite" );
      assertEqual( pv.size(), 2 );
      args.clear();
      args["name"] = "TestSuite";
      args["xml:id"] = "p0";      // duplicate ID' not!
      assertThrow( test.add_processor( args ), DuplicateIDError );
    }
  }

  {
    startTestSerie( "Provenance - Fail to a document - illegal to set processor on <text>" );
    Document test("xml:id='test'");
    KWargs args;
    args["name"] = "whatever";
    args["generate_id"] = "p0";
    args["version"] = "1";
    args["generator"] = "YES";
    processor *proc = test.add_processor( args );
    test.declare( AnnotationType::TOKEN, "adhoc",
		   "processor='" + proc->id() + "'" );
    args.clear();
    args["processor"] = proc->id();
    assertThrow( test.create_root<Text>( args ), ValueError );
  }

  {
    startTestSerie( "Provenance - Create a document with flat processors - Explicit processor assignment" );
    Document test("xml:id='test'");
    KWargs args;
    args["name"] = "SomeTokeniser";
    args["generate_id"] = "p0";
    args["version"] = "1";
    args["generator"] = "YES";
    processor *proc1 = test.add_processor( args );
    test.declare( AnnotationType::TOKEN, "adhoc",
		   "processor='" + proc1->id() + "'" );
    args.clear();
    args["name"] = "SentenceSplitter";
    args["generate_id"] = "p0";
    args["version"] = "1";
    args["generator"] = "Doesn't matter what we say here";
    processor *proc2 = test.add_processor( args );
    test.declare( AnnotationType::SENTENCE, "adhoc",
		   "processor='" + proc2->id() + "'" );
    args.clear();
    args["xml:id"] = "test.text.1";
    Text *body = test.create_root<Text>( args );
    args.clear();
    args["processor"] = proc2->id();
    args["generate_id"] = body->id();
    Sentence *sentence = body->add_child<Sentence>( args );
    args.clear();
    args["processor"] = proc1->id();
    args["text"] = "hello";
    args["generate_id"] = sentence->id();
    Word *w = sentence->add_child<Word>( args );
    args["text"] = "world";
    sentence->add_child<Word>( args );
    const processor *p = test.get_processor(w->processor());
    assertEqual( p, test.provenance()->index(proc1->id()) );
    test.save( "/tmp/provenance-flat-implicit.2.0.0.folia-1.xml" );
    Document xmlref( fol_path + "examples/tests/provenance-flat-implicit.2.0.0.folia.xml" );
    assertTrue( xmldiff( "/tmp/provenance-flat-implicit.2.0.0.folia-1.xml",
			 fol_path
			 + "examples/tests/provenance-flat-implicit.2.0.0.folia.xml" ) );
  }

  {
    startTestSerie( "Provenance - Create a document with flat processors - Implicit processor assignment" );
    Document test("xml:id='test'");
    KWargs args;
    args["name"] = "SomeTokeniser";
    args["id"] = "p0.1";
    args["version"] = "1";
    args["generator"] = "YES";
    test.add_processor( args );
    test.declare( AnnotationType::TOKEN, "adhoc", "processor='p0.1'" );
    args.clear();
    args["name"] = "SentenceSplitter";
    args["id"] = "p0.2";
    args["version"] = "1";
    args["generator"] = ""; // may be empty too
    test.add_processor( args );
    test.declare( AnnotationType::SENTENCE, "adhoc", "processor='p0.2'" );
    args.clear();
    args["xml:id"] = "test.text.1";
    FoliaElement *body = test.append( new Text(args) );
    args.clear();
    args["generate_id"] = body->id();
    FoliaElement *sentence = body->append( new Sentence( args, &test ) );
    args.clear();
    args["text"] = "hello";
    args["generate_id"] = sentence->id();
    FoliaElement *w = sentence->append( new Word( args, &test ) );
    args["text"] = "world";
    sentence->append( new Word( args, &test ) );
    const processor *p = test.get_processor(w->processor());
    assertEqual( p, test.provenance()->index("p0.1") );
    test.save( "/tmp/provenance-flat-implicit.2.0.0.folia-2.xml" );
    Document xmlref( fol_path + "examples/tests/provenance-flat-implicit.2.0.0.folia.xml" );
    assertTrue( xmldiff( "/tmp/provenance-flat-implicit.2.0.0.folia-2.xml",
			 fol_path
			 + "examples/tests/provenance-flat-implicit.2.0.0.folia.xml" ) );
  }

  {
    startTestSerie( "Provenance - Create a document with flat processors - Explicit multiple processor assignment" );
    Document test("xml:id='test'");
    KWargs args;
    args["name"] = "SomeTokeniser";
    args["id"] = "p0.1";
    args["version"] = "1";
    args["generator"] = "YES";
    test.add_processor( args );
    test.declare( AnnotationType::TOKEN, "adhoc", "processor='p0.1'" );
    args.clear();
    args["name"] = "SentenceSplitter";
    args["id"] = "p0.2";
    args["version"] = "1";
    args["generator"] = "YES";
    test.add_processor( args );
    test.declare( AnnotationType::SENTENCE, "adhoc", "processor='p0.2'" );
    // we declare some extra processors (even though we don't really use them),
    // but this means the annotations will need to serialise an explicit
    // processor= attribute
    args["name"] = "SomeOtherTokeniser";
    args["id"] = "p0.3";
    args["version"] = "1";
    test.add_processor( args );
    test.declare( AnnotationType::TOKEN, "adhoc", "processor='p0.3'" );
    args.clear();
    args["name"] = "OtherSentenceSplitter";
    args["id"] = "p0.4";
    args["version"] = "1";
    args["generator"] = "YES";
    test.add_processor( args );
    test.declare( AnnotationType::SENTENCE, "adhoc", "processor='p0.4'" );
    args.clear();
    args["xml:id"] = "test.text.1";
    FoliaElement *body = test.append( new Text(args) );
    args.clear();
    args["processor"] = "p0.2";
    args["generate_id"] = body->id();
    FoliaElement *sentence = body->append( new Sentence( args, &test ) );
    args.clear();
    args["processor"] = "p0.1";
    args["text"] = "hello";
    args["generate_id"] = sentence->id();
    FoliaElement *w = sentence->append( new Word( args, &test ) );
    args["text"] = "world";
    sentence->append( new Word( args, &test ) );
    const processor *p = test.get_processor(w->processor());
    assertEqual( p, test.provenance()->index("p0.1") );
    test.save( "/tmp/provenance-flat-explicit.2.0.0.folia.xml" );
    assertTrue( xmldiff( "/tmp/provenance-flat-explicit.2.0.0.folia.xml",
			 fol_path
			 + "examples/tests/provenance-flat-explicit.2.0.0.folia.xml" ) );
  }

  {
    startTestSerie( "Provenance - Create a document with flat processors - Implicit multiple processor assignment" );
    Document test("xml:id='test'");
    KWargs args;
    args["name"] = "SomeTokeniser";
    args["id"] = "p0.1";
    args["version"] = "1";
    args["generator"] = "YES";
    test.add_processor( args );
    test.declare( AnnotationType::TOKEN, "adhoc", "processor='p0.1'" );
    args.clear();
    args["name"] = "SentenceSplitter";
    args["id"] = "p0.2";
    args["version"] = "1";
    args["generator"] = "YES";
    test.add_processor( args );
    test.declare( AnnotationType::SENTENCE, "adhoc", "processor='p0.2'" );
    // we declare some extra processors (even though we don't really use them),
    // but this means the annotations will need to serialise an explicit
    // processor= attribute
    args["name"] = "SomeOtherTokeniser";
    args["id"] = "p0.3";
    args["version"] = "1";
    args["generator"] = "YES";
    test.add_processor( args );
    test.declare( AnnotationType::TOKEN, "adhoc", "processor='p0.3'" );
    args.clear();
    args["name"] = "OtherSentenceSplitter";
    args["id"] = "p0.4";
    args["version"] = "1";
    args["generator"] = "YES";
    test.add_processor( args );
    test.declare( AnnotationType::SENTENCE, "adhoc", "processor='p0.4'" );
    args.clear();
    args["xml:id"] = "test.text.1";
    FoliaElement *body = test.append( new Text(args) );
    args.clear();
    args["processor"] = "p0.2";
    args["generate_id"] = body->id();
    FoliaElement *sentence = body->append( new Sentence( args, &test ) );
    args.clear();
    args["text"] = "hello";
    args["generate_id"] = sentence->id();
    const FoliaElement *w = 0;
    assertThrow( w = new Word( args, &test ), NoDefaultError );
    assertTrue( w == 0 );
  }

  {
    startTestSerie( "Provenance - Create a document with nested processors (implicit)" );
    Document doc( "xml:id='test'" );
    KWargs args;
    args["name"] = "TestSuite";
    args["id"] = "p0";
    processor *main_p = doc.add_processor( args );
    args.clear();
    args["name"] = "SomeTokeniser";
    args["id"] = "p0.1";
    args["version"] = "1";
    args["generator"] = "YES";
    doc.add_processor( args, main_p );
    doc.declare( AnnotationType::TOKEN, "adhoc", "processor='p0.1'" );
    args.clear();
    args["name"] = "SentenceSplitter";
    args["id"] = "p0.2";
    args["version"] = "1";
    args["generator"] = "YES";
    doc.add_processor( args, main_p );
    doc.declare( AnnotationType::SENTENCE, "adhoc", "processor='p0.2'" );
    args.clear();
    doc.declare( AnnotationType::TEXT, DEFAULT_TEXT_SET, "processor='p0'" );
    args["xml:id"] = "test.text.1";
    FoliaElement *body = doc.append( new Text(args) );
    args.clear();
    args["generate_id"] = body->id();
    FoliaElement *sentence = body->append( new Sentence( args, &doc ) );
    args.clear();
    args["text"] = "hello";
    args["generate_id"] = sentence->id();
    FoliaElement *w = sentence->append( new Word( args, &doc ) );
    args["text"] = "world";

    sentence->append( new Word( args, &doc ) );
    const processor *p = doc.get_processor(w->processor());
    assertEqual( p, doc.provenance()->index("p0.1") );
    doc.save( "/tmp/provenance-nested-implicit.2.0.0.folia.xml" );
    assertTrue( xmldiff( "/tmp/provenance-nested-implicit.2.0.0.folia.xml",
			 fol_path
			 + "examples/tests/provenance-nested-implicit.2.0.0.folia.xml" ) );
  }

  {
    startTestSerie( "Provenance - Create a document with automatic id assignment" );
    Document doc( "xml:id='test'" );
    //    doc.setdebug(8);
    KWargs args;
    args["name"] = "TestSuite";
    args["id"] = "p0";
    processor *main_p = doc.add_processor( args );
    args.clear();
    args["name"] = "SomeTokeniser";
    args["generate_id"] = "next()";
    args["version"] = "1";
    args["generator"] = "YES";
    processor *sub = doc.add_processor( args, main_p );
    args.clear();
    args["processor"] = sub->id();
    doc.declare( AnnotationType::TOKEN, "adhoc", args );
    args.clear();
    args["name"] = "SentenceSplitter";
    args["generate_id"] = "next()";
    args["version"] = "1";
    args["generator"] = "YES";
    sub = doc.add_processor( args, main_p );
    args.clear();
    args["processor"] = sub->id();
    doc.declare( AnnotationType::SENTENCE, "adhoc", args );
    args.clear();
    args["processor"] = main_p->id();
    doc.declare( AnnotationType::TEXT, DEFAULT_TEXT_SET, args );
    args.clear();
    args["xml:id"] = "test.text.1";
    FoliaElement *body = doc.append( new Text(args) );
    args.clear();
    args["generate_id"] = body->id();
    FoliaElement *sentence = body->append( new Sentence( args, &doc ) );
    args.clear();
    args["text"] = "hello";
    args["generate_id"] = sentence->id();
    FoliaElement *w = sentence->append( new Word( args, &doc ) );
    args["text"] = "world";

    sentence->append( new Word( args, &doc ) );
    const processor *p = doc.get_processor(w->processor());
    assertEqual( p, doc.provenance()->index("p0.1") );
    doc.save( "/tmp/provenance-nested-implicit.2.0.0.folia.xml" );
    assertTrue( xmldiff( "/tmp/provenance-nested-implicit.2.0.0.folia.xml",
			 fol_path
			 + "examples/tests/provenance-nested-implicit.2.0.0.folia.xml" ) );
  }

  {
    startTestSerie( "Provenance - Extend the provenance of an existing document" );
    Document doc( fol_path + "examples/provenance.2.0.0.folia.xml" );
    KWargs args;
    args["name"] = "TestSuite";
    args["id"] = "p0";
    assertThrow( doc.add_processor( args ), DuplicateIDError );
    args.clear();
    args["name"] = "TestSuite";
    args["generate_id"] = "000";
    assertThrow( doc.add_processor( args ), XmlError );
    args.clear();
    args["name"] = "TestSuite";
    args["generate_id"] = "next()";
    args["version"] = "1";
    args["generator"] = "YES";
    assertNoThrow( doc.add_processor( args ) );
    args.clear();
    auto mbv = doc.get_processors_by_name("mbpos");
    assertEqual( mbv.size(), 1 );
    auto frogs = doc.get_processors_by_name("frog");
    assertEqual( frogs.size(), 1 );
    args["name"] = "mbpos";
    args["generate_id"] = "auto()";
    args["version"] = "1";
    const processor *sub = 0;
    assertNoThrow( sub = doc.add_processor( args, frogs[0] ) );
    assertTrue( sub != NULL );
    mbv = doc.get_processors_by_name("mbpos");
    assertEqual( mbv.size(), 2 );
    doc.save( "/tmp/jeetje.xml" );
  }

}
