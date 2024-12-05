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
#include <filesystem>
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

// #define AlignReference LinkReference
// #define Alignment Relation
// #define ComplexAlignment SpanRelation
// #define ComplexAlignmentLayer SpanRelationLayer
// #define AbstractTokenAnnotation AbstractInlineAnnotation
// #define AbstractTokenAnnotation_t AbstractInlineAnnotation_t

extern string fol_path;

void engine_test001a(){
  startTestSerie( " copy a document using Folia Engine " );
  Engine proc;
  // proc.set_debug(true);
  assertNoThrow( proc.init_doc( "tests/example.xml",
				"/tmp/example-p1.xml") );
  if ( proc.ok() ){
    const FoliaElement *res = 0;
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

void engine_test001b(){
  startTestSerie( " copy a document with namespaces using Folia Engine" );
  Engine proc;
  //  proc.set_debug(true);
  assertNoThrow( proc.init_doc( "tests/folia.nsexample", "/tmp/nsexample" ) );
  if ( proc.ok() ){
    const FoliaElement *res = 0;
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

void engine_test001c(){
  startTestSerie( " process an invalid document using Folia Engine" );
  Engine proc;
  //  proc.set_debug(true);
  assertThrow( proc.init_doc( "tests/wrongname.xml" ), XmlError );
}

void loop( Engine& proc ){
  const FoliaElement *res;
  while ( ( res = proc.get_node( "pqrs" ) ) ){
    // search a non-existing node. will barf on the unknown tag
    proc.next();
  }
}

void engine_test001d(){
  startTestSerie( " process an invalid document using Folia Engine" );
  Engine proc;
  //  proc.set_debug(true);
  assertNoThrow( proc.init_doc( "tests/unknowntag.xml" ) );
  if ( proc.ok() ){
    // init should work!
    // reading on should fail at a certain point
#if FOLIA_INT_VERSION >= 219
    assertThrow( loop( proc ), ValueError );
#else
    assertThrow( loop( proc ), XmlError );
#endif
  }
}

void engine_test001e(){
  startTestSerie( " copy a difficult document using Folia Engine " );
  Engine proc;
  //  proc.set_debug(true);
  assertNoThrow( proc.init_doc( "tests/bug-1.xml",
				"/tmp/bug-1.xml") );
  if ( proc.ok() ){
    const FoliaElement *res = 0;
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

void engine_test001f(){
  startTestSerie( " copy a FoLiA 2.0 document using Folia Engine " );
  string infile = fol_path
    + "examples/tests/provenance-nested-implicit.2.0.0.folia.xml";
  Engine proc;
  //  proc.set_debug(true);
  assertNoThrow( proc.init_doc( infile, "/tmp/proc_test001f.xml" ) );
  if ( proc.ok() ){
    const FoliaElement *res = 0;
    while ( (res = proc.get_node( "pqrs" ) ) ){
      // search a non-existing node. makes get_node collect the whole document
      proc.next();
    }
    proc.finish();
    string command = "./tests/foliadiff.sh /tmp/proc_test001f.xml " + infile;
    int stat = system( command.c_str() );
    assertMessage( "/tmp/proc_test001f.xml " + infile + " differ!",
     		   (stat == 0) );
  }
}

void engine_test002a(){
  startTestSerie( " copy a document using Folia Engine (alternative)" );
  Engine proc;
  assertNoThrow( proc.init_doc( "tests/example.xml" ) );
  if ( proc.ok() ){
    const FoliaElement *res = 0;
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

void engine_test002b(){
  startTestSerie( " copy a document with namespace using Folia Engine (alternative)" );
  Engine proc;
  //  proc.set_debug(true);
  assertNoThrow( proc.init_doc( "tests/folia.nsexample" ) );
  if ( proc.ok() ){
    const FoliaElement *res = 0;
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

void engine_test002c(){
  startTestSerie( " copy a speech document using Folia Engine" );
  Engine proc;
  //  proc.set_debug(true);
  assertNoThrow( proc.init_doc( "tests/speechexample.xml" ) );
  if ( proc.ok() ){
    const FoliaElement *res = 0;
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

void engine_test003(){
  startTestSerie( " read a document and adding word and annotations using Foliaprocessor " );
  Engine proc;
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

void engine_test004(){
  startTestSerie( " read a document searching for several nodes " );
  Engine proc;
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
	  const Word *w = 0;
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

void engine_test005a(){
  startTestSerie( " enumerate a document on xml_element nodes " );
  Engine proc;
  //  proc.set_debug(true);
  assertNoThrow( proc.init_doc( "tests/example.xml" ) );
  if ( proc.ok() ){
    xml_tree *result = proc.create_simple_tree("tests/example.xml");
    ofstream os( "/tmp/enum.tree" );
    print( os, result );
    delete result;
#if FOLIA_INT_VERSION >= 216
    int stat = system( "diff /tmp/enum.tree tests/enum.tree.216.ok" );
    assertMessage( "/tmp/enum.tree tests/enum.tree.216.ok differ!",
     		   (stat == 0) );
#else
    int stat = system( "diff /tmp/enum.tree tests/enum.tree.ok" );
    assertMessage( "/tmp/enum.tree tests/enum.tree.ok differ!",
     		   (stat == 0) );
#endif
  }
}

#if FOLIA_INT_VERSION >= 221
void engine_test005b(){
  startTestSerie( " enumerate a document on xml_element nodes with debugging" );
  Engine proc;
  Document d("debug='PARSING|SERIALIZE'");
  ofstream os( "/tmp/foliaengine.dbg" );
  TiCC::LogStream ds( os );
  ds.set_level( LogHeavy );
  proc.set_debug(true);
  proc.set_dbg_stream( &ds );
  assertNoThrow( proc.init_doc( "tests/example.xml" ) );
  if ( proc.ok() ){
    xml_tree *result = proc.create_simple_tree("tests/example.xml");
    ofstream os( "/tmp/enum.tree" );
    print( os, result );
    delete result;
    int stat = system( "diff /tmp/enum.tree tests/enum.tree.216.ok" );
    assertMessage( "/tmp/enum.tree tests/enum.tree.216.ok differ!",
     		   (stat == 0) );
  }
  size_t size = std::filesystem::file_size("/tmp/foliatest.dbg");
  assertTrue( size > 5000 );
}
#else
void engine_test005b(){
  // noop
}
#endif

void engine_test006a(){
  startTestSerie( " enumerate a document on text node parents" );
  TextEngine proc;
  //  proc.set_debug(true);
  assertNoThrow( proc.init_doc( "tests/example.xml" ) );
  if ( proc.ok() ){
    map<int,int> result = proc.enumerate_text_parents();
    ofstream os( "/tmp/textparents.lst" );
    for ( const auto& n : result ){
      os << n.first << endl;
    }
#if FOLIA_INT_VERSION >= 216
    int stat = system( "diff /tmp/textparents.lst tests/textparents.lst.216.ok" );
    assertMessage( "/tmp/textparents.lst tests/textparents.lst.216.ok differ!",
     		   (stat == 0) );
#else
    int stat = system( "diff /tmp/textparents.lst tests/textparents.lst.ok" );
    assertMessage( "/tmp/textparents.lst tests/textparents.lst.ok differ!",
     		   (stat == 0) );
#endif
  }
}

void engine_test006b(){
  startTestSerie( " enumerate a document on text node parents with textclass" );
  TextEngine proc;
  //  proc.set_debug(true);
  assertNoThrow( proc.init_doc( "tests/example.xml" ) );
  if ( proc.ok() ){
    map<int,int> result = proc.enumerate_text_parents("original");
    ofstream os( "/tmp/textparents-o.lst" );
    for ( const auto& n : result ){
      os << n.first << endl;
    }
#if FOLIA_INT_VERSION >= 216
    int stat = system( "diff /tmp/textparents-o.lst tests/textparents-o.lst.216.ok" );
    assertMessage( "/tmp/textparents-o.lst tests/textparents-o.lst.216.ok differ!",
     		   (stat == 0) );
#else
    int stat = system( "diff /tmp/textparents-o.lst tests/textparents-o.lst.ok" );
    assertMessage( "/tmp/textparents-o.lst tests/textparents-o.lst.ok differ!",
     		   (stat == 0) );
#endif
  }
}

void engine_test006c(){
  startTestSerie( " enumerate a document with layers on text node parents" );
  TextEngine proc;
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

void engine_test006d(){
  startTestSerie( " enumerate a complex document on text node parents" );
  TextEngine proc;
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

void engine_test007(){
  startTestSerie( " process a document searching for text nodes " );
  TextEngine proc;
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

void engine_test008a(){
  startTestSerie( " process a document searching for text nodes " );
  TextEngine proc;
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

void engine_test008b(){
  startTestSerie( " process a document with layers searching for text nodes " );
  TextEngine proc;
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

void engine_test008c(){
  startTestSerie( " process a complex document searching for text nodes " );
  TextEngine proc;
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

void test_proc( TextEngine& proc ){
  const FoliaElement *e = 0;
  while ( (e = proc.next_text_parent() ) ){
  }
}

void engine_test008d(){
  startTestSerie( " process a strange document searching for text nodes " );
  TextEngine proc( "tests/scary.xml", "/tmp/proctest-8d.xml");
  if ( proc.ok() ){
    proc.setup("",true);
    assertThrow( test_proc( proc ), XmlError );
  }
}

void engine_test008e(){
  startTestSerie( " process a document with entities, searching for text nodes " );
  TextEngine proc( "tests/entities.xml" );
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

void engine_test009a(){
  startTestSerie( " process a difficult document with t-style " );
  TextEngine proc;
  //  proc.set_debug(true);
  proc.init_doc( "tests/procbug.xml" );
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

void engine_test009b(){
  startTestSerie( " process a difficult document for text " );
  TextEngine proc( "tests/textbug1.xml" );
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

void engine_test009c(){
  startTestSerie( " process a difficult document for text " );
  TextEngine proc;
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

void engine_test009d(){
  startTestSerie( " process a tagged text document " );
  TextEngine proc;
  assertNoThrow( proc.init_doc( "tests/tagged.xml" ) );
  //  proc.set_debug(true);
  ofstream os( "/tmp/testtagged.out" );
  if ( proc.ok() ){
    proc.setup("",true);
    FoliaElement *e = 0;
    assertNoThrow( e = proc.next_text_parent() );
    while ( e ) {
      os << e->id() << " : " << e->str() << endl;
      assertNoThrow( e = proc.next_text_parent() );
    }
    os << "done" << endl;
    int stat = system( "diff /tmp/testtagged.out tests/testtagged.ok" );
    assertMessage( "/tmp/testtagged.out tests/testtagged.ok differ!",
		   (stat == 0) );
  }
}

void engine_test010(){
  startTestSerie( " copy a document using Folia Engine on string buffer" );
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

  Engine proc;
  // proc.set_debug(true);
  assertNoThrow( proc.init_doc( xml ) );
  if ( proc.ok() ){
    const FoliaElement *res = 0;
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

void engine_test011() {
  startTestSerie( " Test lezen en schrijven van een BZ2 FoLiA file " );
  Engine proc( "/tmp/example.xml.bz2" ); // created by earlier test
  while ( proc.get_node( "pqrs" ) ){
    // search a non-existing node. makes get_node collect the whole document
    proc.next();
  }
  proc.save( "/tmp/example-1.xml.bz2" );
  int stat = system( "./tests/foliadiff.sh /tmp/example-1.xml.bz2 /tmp/example.xml.bz2" );
  assertMessage( "/tmp/example-1.xml.bz2 tests/example.xml.bz2 differ!",
   		 (stat == 0) );
}

void engine_test012() {
  startTestSerie( " Test reading of erroneous files" );
  string err_path = fol_path + "examples/erroneous/";
  {
    Engine proc;
    //    proc.set_debug(true);
    assertThrow( proc.init_doc( err_path + "syntax_error_a.2.2.1.folia.xml" ),
		 XmlError );
  }
  {
    Engine proc;
    //    proc.set_debug(true);
    assertNoThrow( proc.init_doc( err_path + "syntax_error_b.2.2.1.folia.xml" ) );
    if ( proc.ok() ){
      assertThrow( loop( proc ), XmlError );
    }
  }
  {
    Engine proc;
    //    proc.set_debug(true);
    assertNoThrow( proc.init_doc( err_path + "syntax_error_c.2.2.1.folia.xml" ) );
    if ( proc.ok() ){
      assertThrow( loop( proc ), XmlError );
    }
  }
  {
    Engine proc;
    //    proc.set_debug(true);
    assertNoThrow( proc.init_doc( err_path + "syntax_error_d.2.2.1.folia.xml" ) );
    if ( proc.ok() ){
      assertThrow( loop( proc ), XmlError );
    }
  }
}

void engine_test013() {
  startTestSerie( " Test reading of a file with PI's" );
#if FOLIA_INT_VERSION >= 216
  string path = "tests/PI.xml";
  {
    Engine proc;
    //    proc.set_debug(true);
    assertNoThrow( proc.init_doc( path ) );
    if ( proc.ok() ){
      FoliaElement *res = 0;
      while ( (res = proc.get_node( "pqrs" ) ) ){
	// search a non-existing node. makes get_node collect the whole document
	proc.next();
      }
      assertNoThrow( proc.save( "/tmp/PI.xml") );
      int stat = system( "./tests/foliadiff.sh /tmp/PI.xml tests/PI.xml" );
      assertMessage( "/tmp/PI.xml tests/PI.xml differ!",
		     (stat == 0) );
    }
  }
  {
    Engine proc;
    //    proc.set_debug(true);
    assertNoThrow( proc.init_doc( path ) );
    if ( proc.ok() ){
      FoliaElement *res = proc.get_node( "p" );
      auto piv = res->getPI();
      assertEqual( piv.size(), 3 );
    }
  }
  {
    Engine proc;
    //    proc.set_debug(true);
    assertNoThrow( proc.init_doc( path ) );
    if ( proc.ok() ){
      FoliaElement *res = 0;
      ofstream os( "/tmp/PInodes.txt" );
      while ( (res = proc.get_node( "PI" ) ) ){
	auto PI = dynamic_cast<ProcessingInstruction*>( res );
	os << PI->target() << " : " << PI->content() << endl;
      }
      int stat = system( "diff /tmp/PInodes.txt tests/PInodes.txt.ok" );
      assertMessage( "/tmp/PInodes.txt tests/PInodes.txt.ok differ!",
		     (stat == 0) );
    }
  }
  {
    Engine proc;
    //  proc.set_debug(true);
    assertNoThrow( proc.init_doc( "tests/PI.xml" ) );
    if ( proc.ok() ){
      xml_tree *result = proc.create_simple_tree("tests/PI.xml");
      ofstream os( "/tmp/PIenum.tree" );
      print( os, result );
      delete result;
      int stat = system( "diff /tmp/PIenum.tree tests/PIenum.tree.ok" );
      assertMessage( "/tmp/PIenum.tree tests/PIenum.tree.ok differ!",
		     (stat == 0) );
    }
  }
#endif
}
