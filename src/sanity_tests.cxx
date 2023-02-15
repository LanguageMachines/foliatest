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

Document sanityDoc( "file='tests/example.xml'" );

void sanity_test000( ){
  startTestSerie( " Text count " );
  assertTrue( sanityDoc.getRoot()->isinstance<Text>() );
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
  assertEqual( s->text(TEXT_FLAGS::RETAIN), "De andere handschriften krijgen ook een letter die verband kan houden met hun plaats van oorsprong óf plaats van bewaring ." );
  // not detokenised
  assertEqual( s->toktext(), "De andere handschriften krijgen ook een letter die verband kan houden met hun plaats van oorsprong óf plaats van bewaring ." );
  // just an alias for the above
}

void sanity_test006c(){
  startTestSerie( " Sanity check - Sentence text both tokenized and not " );
  // grab fourth sentence
  Sentence *s = sanityDoc.sentences(4);
  //  assertTrue( s->hastext() );
  // The sentence has text of it's own
  //  assertEqual( s->text(), "De hoofdletter A wordt gebruikt voor het originele handschrift." );
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
  assertNoThrow( d = sanityDoc["sandbox.figure.1"] );
  FoliaElement *e = 0;
  assertNoThrow( e = new Caption( getArgs("xml:id='whatever'"), &sanityDoc ) );
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
  assertNoThrow( doc.read_from_string(xml) );
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
#if FOLIA_INT_VERSION < 23
  assertThrow( w->annotation<SenseAnnotation>(),
	       NoSuchAnnotation );
#else
  auto a = w->annotation<SenseAnnotation>();
  assertTrue( a == 0 );
#endif

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
  assertNoThrow( t = w->text_content() );
  assertNoThrow( t = t->text_content() ); // should deliver itself!
  assertTrue( t->text() == "hoofdletter" );
  assertTrue( t->offset() == 3 );

  FoliaElement *w2= sanityDoc["WR-P-E-J-0000000001.p.1.s.6.w.31"];
  assertTrue( w2->text() == "vierkante" );
  assertTrue( w2->stricttext() == "vierkante" );

}

void sanity_test030b( ){
  startTestSerie( " Sanity check - Text Content (2) " );
  FoliaElement *head = sanityDoc["sandbox.3.head"];
  const TextContent *t = head->text_content();
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
  assertNoThrow( doc.read_from_string(xml) );
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
  assertNoThrow( doc.read_from_string(xml) );
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
  assertNoThrow( doc.read_from_string(xml) );
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
#if FOLIA_INT_VERSION < 23
  PosAnnotation *p =0;
  assertThrow( p = sanityDoc["WR-P-E-J-0000000001.sandbox.2.s.1.w.2"]->annotation<PosAnnotation>(), NoSuchAnnotation );
#else
  PosAnnotation *p = sanityDoc["WR-P-E-J-0000000001.sandbox.2.s.1.w.2"]->annotation<PosAnnotation>();
#endif
  assertTrue( p == 0 );
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
    for ( const auto& w: span->wrefs() ){
      res += " " + w->text();
    }
  }
  assertEqual( res, " ander woord" );
  assertNoThrow( spans = word->parent()->selectSpan() );
  assertEqual( spans.size(), 24 );
}

void sanity_test041b(){
  startTestSerie( "Sanity check - Find spans given words (specific set)" );
  UnicodeString res;
  FoliaElement *word = sanityDoc["example.table.1.w.3"];
  vector<AbstractSpanAnnotation*> spans;
  assertNoThrow( spans = word->findspans<EntitiesLayer>("http://raw.github.com/proycon/folia/master/setdefinitions/namedentities.foliaset.xml") );
  for( const auto& span: spans ){
    for ( const auto& w: span->wrefs() ){
      res += " " + w->text();
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
    for ( const auto& w: span->wrefs() ){
      res += " " + w->text();
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
  assertTrue( head->has_annotation<String>() > 0 );
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
  const TextContent *t = head->text_content();
  assertEqual( len(head->select<TextMarkupString>()), size_t(1) );
  assertEqual( len(t->select<TextMarkupString>()), size_t(1) );
  vector<TextMarkupString*> v = t->select<TextMarkupString>();
  TextMarkupString *st = v[0];
  // testing value (full text value)
  assertEqual( st->text(), "\nFoLiA developers" ) ;
  const FoliaElement *r1 =  st->resolveid();
  FoliaElement *r2 = sanityDoc["sandbox.3.str"];
  assertEqual( r1, r2 ); // testing resolving references
  t = sanityDoc["WR-P-E-J-0000000001.p.1.s.6"]->text_content();
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
  assertEqual( txt, "Term uit de editiewetenschap en tekstkritiek voor de schematische weergave in de vorm van een boomdiagram van de onderlinge verwantschap van\ndocumentaire bronnen (handschrift, codex, druk), waarin een tekst bewaard is gebleven.\n" );
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

void sanity_test100( ){
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
  assertEqual( doc.metadata_type(), "cmdi" );
  assertEqual( doc.metadata_file(), "test.cmdi.xml" );
  assertNoThrow( doc.save( "/tmp/folia.cmdi.xml" ) );
  int stat = system( "./tests/foliadiff.sh /tmp/folia.cmdi.xml tests/folia.cmdi.xml" );
  assertMessage( "/tmp/folia.cmdi.xml tests/folia.cmdi.xml differ!",
		 stat == 0 );
  assertThrow( doc.set_metadata( "language", "por" ), MetaDataError );
}

void sanity_test101b(){
  startTestSerie(" Metadata external reference (IMDI) " );
  Document doc( "file='tests/folia.imdi.xml'" );
  assertEqual( doc.metadata_type(), "imdi" );
  assertEqual( doc.metadata_file(), "" );
  assertNoThrow( doc.save( "/tmp/folia.imdi.xml" ) );
  int stat = system( "./tests/foliadiff.sh /tmp/folia.imdi.xml tests/folia.imdi.xml" );
  assertMessage( "/tmp/folia.imdi.xml tests/folia.imdi.xml differ!",
		 stat == 0 );
  assertThrow( doc.set_metadata( "language", "por" ), MetaDataError );
}

void sanity_test101c(){
  startTestSerie(" Metadata (native) " );
  Document doc( "file='tests/example.xml'" );
  assertTrue( doc.metadata_type() == "native" );
  assertNoThrow( doc.set_metadata( "name", "Mijn document" ) );
  assertEqual( doc.get_metadata( "genre" ), "artikel" );
}

void sanity_test101d(){
  startTestSerie(" Metadata (foreign) " );
  Document doc( "file='tests/folia.foreign.xml'" );
  assertTrue( doc.metadata_type() == "pm" );
  assertNoThrow( doc.save( "/tmp/saveforeign.xml" ) );
  int stat = system( "./tests/foliadiff.sh /tmp/saveforeign.xml tests/folia.foreign.xml" );
  assertMessage( "/tmp/saveforeign.xml tests/folia.foreign.xml differ!",
		 stat == 0 );
}

void sanity_test101e(){
  startTestSerie(" Metadata (foreign with namespace) " );
  Document doc( "file='tests/folia.foreign2.xml'" );
  assertTrue( doc.metadata_type() == "pm" );
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
#if !(FOLIA_INT_VERSION >= 27)
  assertThrow( f_doc.set_metadata( "language", "por" ), MetaDataError );
#else
  assertNoThrow( f_doc.set_metadata( "language", "por" ) );
#endif
  assertNoThrow( f_doc.save( "/tmp/foreignmeta3.out" ) );
#if !(FOLIA_INT_VERSION >= 27)
  int stat2 = system( "./tests/foliadiff.sh /tmp/foreignmeta3.out tests/foreignmeta3.out" );
  assertMessage( "/tmp/foreignmeta3.out tests/foreignmeta3.out differ!",
		 stat2 == 0 );
#else
  int stat2 = system( "./tests/foliadiff.sh /tmp/foreignmeta3.out tests/foreignmeta3_v25.out" );
  assertMessage( "/tmp/foreignmeta3.out tests/foreignmeta3_v25.out differ!",
		 stat2 == 0 );
#endif
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
  assertNoThrow( doc.read_from_string(xml) );
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
  assertThrow( doc.read_from_string(xml), DeclarationError );

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
  assertNoThrow( doc.read_from_string(xml) );
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
  assertThrow( doc.read_from_string(xml), DeclarationError );

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
  assertThrow( doc.read_from_string(xml), DeclarationError );

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
  assertNoThrow( doc.read_from_string(xml) );
  assertTrue( doc.default_set(AnnotationType::GAP) == "gap-set" );
  assertTrue( doc.default_annotator(AnnotationType::GAP) == "sloot" );
  assertTrue( doc.default_annotator(AnnotationType::GAP,"gap-set") == "sloot" );

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
  assertThrow( doc.read_from_string(xml), DeclarationError );

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
  assertNoThrow( doc.read_from_string(xml) );
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
  assertNoThrow( doc.read_from_string(xml) );
  vector<Gap*> v = doc["example.text.1"]->select<Gap>();
  assertTrue( v[0]->description() == "test1" );
  assertTrue( v[0]->sett() == "undefined" );
  assertTrue( v[1]->description() == "test2" );
  assertTrue( v[1]->sett() == "undefined" );
  assertTrue( v[1]->xmlstring() == "<gap xmlns=\"http://ilk.uvt.nl/folia\" class=\"Y\"><desc>test2</desc></gap>" );

}

void sanity_test102h(){
  startTestSerie(" Declarations - Ambiguous set" );
  string xml = R"~(<?xml version="1.0"?>
 <FoLiA xmlns:xlink="http://www.w3.org/1999/xlink"
xmlns="http://ilk.uvt.nl/folia" xml:id="example" generator="libfolia-v0.8" version="0.8">
  <metadata type="native">
    <annotations>
      <gap-annotation annotator="sloot" set="gap-set"/>
      <gap-annotation annotator="proycon" set="gap-set2"/>
    </annotations>
  </metadata>
  <text xml:id="example.text.1">
    <gap class="X" />
  </text>
</FoLiA>)~";

  Document doc;
  assertThrow( doc.read_from_string(xml), DeclarationError );
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
  assertNoThrow( doc.read_from_string(xml) );
  assertTrue( doc.default_annotator(AnnotationType::GAP,"gap1-set") == "sloot" );
  assertNoThrow( doc.declare( AnnotationType::GAP,
					"gap1-set",
					"annotator='proycon'" ) );
#if FOLIA_INT_VERSION >= 211
  assertEqual( doc.default_annotator(AnnotationType::GAP,"gap1-set"), "proycon" );
#else
  assertEqual( doc.default_annotator(AnnotationType::GAP,"gap1-set"), "" );
#endif
  assertEqual( doc.default_annotator(AnnotationType::GAP,"gap2-set"), "sloot" );
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
  assertEqual( v[0]->xmlstring(), "<gap xmlns=\"http://ilk.uvt.nl/folia\" annotator=\"sloot\" class=\"X\" set=\"gap1-set\"/>" );
#if FOLIA_INT_VERSION >= 211
  assertEqual( v[1]->xmlstring(), "<gap xmlns=\"http://ilk.uvt.nl/folia\" class=\"Y\" set=\"gap1-set\"/>" );
#else
  assertEqual( v[1]->xmlstring(), "<gap xmlns=\"http://ilk.uvt.nl/folia\" annotator=\"proycon\" class=\"Y\" set=\"gap1-set\"/>" );
#endif
  assertEqual( v[2]->xmlstring(), "<gap xmlns=\"http://ilk.uvt.nl/folia\" class=\"Z1\" set=\"gap1-set\"/>" );
  assertEqual( v[3]->xmlstring(), "<gap xmlns=\"http://ilk.uvt.nl/folia\" class=\"Z2\" set=\"gap2-set\"/>" );
  assertEqual( v[4]->xmlstring(), "<gap xmlns=\"http://ilk.uvt.nl/folia\" annotator=\"onbekend\" class=\"Y2\" set=\"gap2-set\"/>" );
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
  assertNoThrow( doc.read_from_string(xml) );
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
  assertNoThrow( doc.read_from_string(xml) );
  FoliaElement *text = doc["example.text.1"];
  assertEqual( doc.default_annotatortype(AnnotationType::GAP), AUTO );
  vector<Gap*> v = text->select<Gap>();
  assertEqual( v[0]->xmlstring(), "<gap xmlns=\"http://ilk.uvt.nl/folia\" class=\"X\"/>" );
  assertNoThrow( doc.declare( AnnotationType::GAP,
			      "gap-set",
			      "annotatortype='manual'" ) );
#if FOLIA_INT_VERSION >= 211
  assertEqual( doc.default_annotatortype(AnnotationType::GAP), MANUAL );
#else
  assertEqual( doc.default_annotatortype(AnnotationType::GAP), UNDEFINED );
#endif
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
#if FOLIA_INT_VERSION >= 211
  assertEqual( v[0]->xmlstring(), "<gap xmlns=\"http://ilk.uvt.nl/folia\" annotatortype=\"auto\" class=\"X\"/>" );
  assertEqual( v[1]->xmlstring(), "<gap xmlns=\"http://ilk.uvt.nl/folia\" class=\"Y\"/>" );
  assertEqual( v[2]->xmlstring(), "<gap xmlns=\"http://ilk.uvt.nl/folia\" annotatortype=\"auto\" class=\"Z\"/>" );
#else
  assertEqual( v[0]->xmlstring(), "<gap xmlns=\"http://ilk.uvt.nl/folia\" annotatortype=\"auto\" class=\"X\" set=\"gap-set\"/>" );
  assertEqual( v[1]->xmlstring(), "<gap xmlns=\"http://ilk.uvt.nl/folia\" annotatortype=\"manual\" class=\"Y\" set=\"gap-set\"/>" );
  assertEqual( v[2]->xmlstring(), "<gap xmlns=\"http://ilk.uvt.nl/folia\" annotatortype=\"auto\" class=\"Z\" set=\"gap-set\"/>" );
#endif
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
  assertNoThrow( doc.read_from_string(xml) );
  FoliaElement *text = doc["example.text.1"];
  assertTrue( doc.default_datetime(AnnotationType::GAP,"gap-set") == "2012-06-18T17:49:00" );
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
  assertNoThrow( doc.read_from_string(xml) );
  assertNoThrow( doc.declare( AnnotationType::GAP,
			      "gap-set2",
			      "annotatortype='manual'" ) );
  assertNoThrow( doc.save( "/tmp/declared1.out" ) );
  assertNoThrow( doc.un_declare( AnnotationType::GAP, "gap-set2" ) );
  assertNoThrow( doc.save( "/tmp/declared2.out" ) );
  assertThrow( doc.un_declare( AnnotationType::GAP, "gap-set" ),
	       DeclarationError );
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
  assertNoThrow( doc.read_from_string(xml) );
  assertNoThrow( doc.declare( AnnotationType::GAP,
			      "nog zon ingewikkelde en veels te lange declaratie",
			      "annotatortype='manual', alias='gap-set2'" ) );
  assertNoThrow( doc.save( "/tmp/declared1.out" ) );
  // using a setname which is already an alias is a error
  assertThrow( doc.declare( AnnotationType::GAP, "gap-set3", "alias='gap-set2'"),
	       DeclarationError );
  // using an alias  which is already an alias is a error
  assertThrow( doc.declare( AnnotationType::GAP, "someset", "alias='gap-set'"),
	       DeclarationError );
  // using an alias  which is already a setname is a error
  assertThrow( doc.declare( AnnotationType::GAP, "some_other_set", "alias='nog zon ingewikkelde en veels te lange declaratie'"),
	       DeclarationError );
  // just declaring again is NOT an error!
  assertNoThrow( doc.declare( AnnotationType::GAP,
			      "nog zon ingewikkelde en veels te lange declaratie",
			      "annotatortype='manual', alias='gap-set2'" ) );
  // declaring again with another alias IS an error!
  assertThrow( doc.declare( AnnotationType::GAP,
			    "nog zon ingewikkelde en veels te lange declaratie",
			    "annotatortype='manual', alias='gap-set3'" ),
	       DeclarationError) ;
  // declaring again with same alias and another setname IS an error!
  assertThrow( doc.declare( AnnotationType::GAP,
			    "niet zon ingewikkelde en veels te lange declaratie",
			    "annotatortype='manual', alias='gap-set2'" ),
	       DeclarationError );
  assertNoThrow( doc.un_declare( AnnotationType::GAP, "gap-set2" ) );
  assertNoThrow( doc.save( "/tmp/declared2.out" ) );
  assertThrow( doc.un_declare( AnnotationType::GAP, "gap-set" ),
	       DeclarationError );
  assertNoThrow( doc.save( "/tmp/declared3.out" ) );
}

void sanity_test102o(){
  startTestSerie(" Declarations - using same sets and aliases for different types." );
  Document doc;
  assertNoThrow( doc.read_from_file("tests/aliases.xml") );
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
  assertNoThrow( doc.read_from_string(xml) );
  assertEqual( len(doc.words()),  1 ); // first word is in alien namespace
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
  assertNoThrow( doc.read_from_string(xml) );
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
  assertNoThrow( doc.read_from_string(xml) );
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
  assertNoThrow( doc.read_from_string(xml) );
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
  assertNoThrow( doc.read_from_string(xml) );
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
  assertNoThrow( doc.read_from_file("tests/fg.xml") );
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
  assertThrow( doc.read_from_string(xml), XmlError );  // invalid id!
  xml = "<?xml version=\"1.0\"?>\n"
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\" "
"xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"voorbeeld_1\" generator=\"libfolia-v0.8\" version=\"0.8\">\n"
"</FoLiA>\n" ;
  assertNoThrow( doc.read_from_string(xml) );
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
  auto sm = doc["sandbox.3"]->get_metadata();
  assertEqual( sm->datatype(), "NativeMetaData" );
  KWargs att = sm->get_avs();
  assertEqual( att["author"], "proycon" );
  sm = doc["example.table.1.w.1"]->get_metadata();
  assertEqual( sm->datatype(), "NativeMetaData" );
  att = sm->get_avs();
  assertEqual( att["author"], "proycon" );
  string val = doc["example.table.1.w.1"]->get_metadata("author");
  assertEqual( val, "proycon" );
  sm = doc["WR-P-E-J-0000000001.div0.1"]->get_metadata();
  assertEqual( sm->datatype(), "NativeMetaData" );
  att = sm->get_avs();
  assertEqual( att["originalsource"], "https://nl.wikipedia.org/wiki/Stemma" );
  sm = doc["WR-P-E-J-0000000001.p.1.s.1.w.1"]->get_metadata();
  assertEqual( sm->datatype(), "NativeMetaData" );
  att = sm->get_avs();
  assertEqual( att["originalsource"], "https://nl.wikipedia.org/wiki/Stemma" );
  val = doc["WR-P-E-J-0000000001.div0.1"]->get_metadata("originalsource");
  assertEqual( val, "https://nl.wikipedia.org/wiki/Stemma" );
  sm = doc["span.correction"]->get_metadata();
  assertEqual( sm->datatype(), "ExternalMetaData" );
  assertEqual( sm->src(), "blaat" );
  sm = doc["bibhead"]->get_metadata();
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
  assertThrow( d.read_from_string( xml ), XmlError );
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

  //  assertThrow( d.read_from_string( xml ), XmlError );
  assertNoThrow( d.read_from_string( xml ) );
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

  assertThrow( d.read_from_string( xml ), XmlError );
}

void sanity_test123( ){
  startTestSerie( " Layers - using Word not Reference " );
  Document d;
  assertThrow( d.read_from_file( "tests/example_2.xml" ), XmlError );
}

void sanity_test130( ){
  startTestSerie( " selection " );
  Document d( "tests/selection_test.xml" );
  auto root = d.doc();
  root=root->index(0); // Text
  vector<Division*> dv = root->select<Division>();
  assertEqual( dv.size(), 5 );
  dv = root->select<Division>(false);
  assertEqual( dv.size(), 2 );
  dv = root->select<Division>("role");
  assertEqual( dv.size(), 3 );
  int cnt1 = 0;
  int cnt2 = 0;
  for ( auto const& it : dv ){
    if ( it->cls() == "speaker" ) ++cnt1;
    if ( it->cls() == "crowd" ) ++cnt2;
  }
  assertEqual( cnt1, 2 );
  assertEqual( cnt2, 1 );
  dv = root->select<Division>("structure");
  assertEqual( dv.size(), 2 );
  cnt1 = 0;
  cnt2 = 0;
  for ( auto const& it : dv ){
    if ( it->cls() == "chapter" ) ++cnt1;
    if ( it->cls() == "glossary" ) ++cnt2;
  }
  assertEqual( cnt1, 1 );
  assertEqual( cnt2, 1 );
  dv = root->select<Division>("chapter");
  assertEqual( dv.size(), 0 );
  dv = root->select<Division>("speaker");
  assertEqual( dv.size(), 0 );
  dv = root->select<Division>("crowd");
  assertEqual( dv.size(), 0 );
}

#if FOLIA_INT_VERSION >= 29
void sanity_test140( ){
  startTestSerie( " test parsing of xml:space attribute ");
  Document doc( "tests/xmlspace.xml" );
  doc.save( "/tmp/xmlspace.xml" );
  int stat = system( "./tests/foliadiff.sh /tmp/xmlspace.xml tests/xmlspace.xml" );
  assertMessage( "/tmp/xmlspace.xml tests/xmlspace.xml differ!",
   		 (stat == 0) );
}

void sanity_test141( ){
  startTestSerie( " test creating a doc with xml:space attribute ");
  Document doc( "xml:id='spaces'" );
  assertNoThrow( doc.declare( AnnotationType::DIVISION, "myset" ) );
  assertNoThrow( doc.declare( AnnotationType::PARAGRAPH, "myset" ) );
  assertNoThrow( doc.declare( AnnotationType::SENTENCE, "myset" ) );
  KWargs args;
  args["xml:id"] = "text";
  FoliaElement *root = doc.addText( args );
  args.clear();
  args["generate_id"] = "text";
  Division *d1 = root->add_child<Division>( args );
  args["generate_id"] = d1->id();
  args["xml:space"] = "preserve";
  Paragraph *p1 = d1->add_child<Paragraph>( args );
  args.clear();
  args["generate_id"] = p1->id();
  Sentence *s1 = p1->add_child<Sentence>( args );
  s1->settext( "de kat\nis    aaibaar" );
  args.clear();
  args["generate_id"] = p1->id();
  args["xml:space"] = "default";
  Sentence *s2 = p1->add_child<Sentence>( args );
  s2->settext( "de hond\nblaft    hard" );
  doc.setmode( "strip" );
  doc.save( "/tmp/test141.xml", true );
  int stat = system( "diff /tmp/test141.xml tests/test141.xml.ok" );
  assertMessage( "/tmp/test141.xml tests/test141.xml.ok differ!",
   		 (stat == 0) );
}
#endif

void sanity_test150( ){
  startTestSerie( " Reading a document with missing processor declaration " );
  Document d;
  assertThrow( d.read_from_file( "tests/missing_proc.xml" ), XmlError );
}
