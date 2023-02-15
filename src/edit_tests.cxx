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
#include "cassert"
using namespace std;
using namespace icu;
using namespace folia;

using TiCC::operator<<;

#define AlignReference LinkReference
#define Alignment Relation

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
  assertThrow( w->addAnnotation<PosAnnotation>( args ), DeclarationError );
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
  string pos_set = "https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn";
  KWargs args;
  args["class"] = "V";
  args["set"] = pos_set;
  assertNoThrow( w->addPosAnnotation( args ) );
  vector<Alternative*> alt = w->alternatives(); // all alternatives
  vector<Alternative*> alt2 = w->alternatives(pos_set);
  assertTrue( alt.size() == 1 );
  assertTrue( alt2.size() == 1 );
  assertTrue( alt[0] == alt2[0] );
  FoliaElement *p = 0;
  assertTrue( p = w->annotation<PosAnnotation>( pos_set ) );
  assertTrue( p->isinstance<PosAnnotation>() );

  std::vector<Alternative *> alt3;
  assertNoThrow( alt3 = w->alternatives(PosAnnotation_t, pos_set) );
  assertEqual( alt3.size(), 1 );
  assertEqual( alt[0] , alt3[0] );

  assertEqual( w->xmlstring(), "<w xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.2.w.11\"><t>naam</t><pos class=\"N(soort,ev,basis,zijd,stan)\" set=\"https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn\"/><lemma class=\"naam\"/><alt xml:id=\"WR-P-E-J-0000000001.p.1.s.2.w.11.alt-pos.1\" auth=\"no\"><pos class=\"V\" set=\"https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn\"/></alt></w>" );

  std::vector<PosAnnotation*> pv;
  PosAnnotation *pos1 = w->getPosAnnotations( pos_set, pv );
  assertMessage( "No pos annotation found!", (pos1 != 0) );
  assertEqual( pv.size(), 1 );
  if ( pos1 != 0 ){
    assertEqual( pos1->xmlstring(), "<pos xmlns=\"http://ilk.uvt.nl/folia\" class=\"N(soort,ev,basis,zijd,stan)\" set=\"https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn\"/>" );
    assertEqual( pv[0]->xmlstring(), "<pos xmlns=\"http://ilk.uvt.nl/folia\" class=\"V\" set=\"https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn\"/>" );
  }
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
  FoliaElement *c = w->annotation<Correction>();
  assertMessage( "No correction found!", (c != 0) );
  if ( c ){
    assertEqual( c->getOriginal()->index(0) , oldpos );
    assertEqual( (*c->getNew())[0],  newpos );
  }
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
  kw["xml:id"] = "WR-P-E-J-0000000001.p.1.s.8.w.11";
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
  MorphologyLayer *l = new MorphologyLayer();
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
  assertEqual( res, "<w xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.6.w.8\"><t>ze</t><pos class=\"VNW(pers,pron,stan,red,3,mv)\" set=\"https://raw.githubusercontent.com/proycon/folia/master/setdefinitions/frog-mbpos-cgn\"/><lemma class=\"ze\"/><alignment class=\"coreference\"><aref id=\"WR-P-E-J-0000000001.p.1.s.6.w.1\" t=\"appel\" type=\"w\"/><aref id=\"WR-P-E-J-0000000001.p.1.s.6.w.2\" type=\"w\"/></alignment></w>" );
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
  vector<PosAnnotation*> v = word->annotations<PosAnnotation>();
  assertEqual( v.size(), 0 );
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
  // 2 get the text of the Sentence itself (it contains spaces and tabs that are normalized away since FoLiA >= 2.5)!
  assertEqual( s->stricttext(), "Is het creëren van een volwaardig literair oeuvre voorbehouden aan schrijvers als Couperus, Haasse, of Grunberg?");
  // 3 try to change the text
  assertThrow( s->settext("This MUST fail!" ), InconsistentText );
  // 4 but we may add different text in another class
  assertNoThrow( s->settext("And now for something completely different.", "larch" ) );
  // 5 get the text from the children
  // 5a check the text in the 'original' class
  assertEqual( s->text("original"), "een volwaardig œuvre" );
  // 5b check the text in the 'larch' class
  assertTrue( s->hastext("larch") );
  assertEqual( s->text("larch"),"And now for something completely different." );
  // 6 get one word
  FoliaElement *w = editDoc["example.p.1.s.1.w.8"];
  // check it's REAL text, but not including leading/trailing spaces
  assertEqual( w->text("original"), "œuvre" );
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
