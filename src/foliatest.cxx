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
#include "libfolia/document.h"
#include "libfolia/folia.h"

#include <ticcutils/UnitTest.h>

using namespace std;
using namespace folia;

void test0() {
  startTestSerie( " Test lezen van KWargs " );
  KWargs bla;
  assertNoThrow( bla = getArgs( "dit='goed', dat =' ra ar' " ) );
  assertTrue( bla["dit"] == "goed" );
  assertTrue( bla["dat"] == " ra ar" );
  assertEqual ( bla["dat"] , string(" ra ar") );
  assertThrow( bla = getArgs( "dit='fout' = " ), ArgsError );
  assertThrow( bla = getArgs( "dit=fout" ), ArgsError );
  assertNoThrow( bla = getArgs( "cls='o\\'k'" ) );
  assertTrue( bla["cls"] == "o'k" );
  assertNoThrow( bla = getArgs( "cls='o\\k'" ) );
  assertTrue( bla["cls"] == "o\\k" );
  assertNoThrow( bla = getArgs( "cls='o\"k'" ) );
  assertTrue( bla["cls"] == "o\"k" );
  assertNoThrow( bla = getArgs( "cls='o""k'" ) );
  assertTrue( bla["cls"] == "ok" );
  assertNoThrow( bla = getArgs( "cls='ok\\a', bli='bla'" ) );
  assertTrue( bla["cls"] == "ok\\a" );
  assertTrue( bla["bli"] == "bla" );
  assertNoThrow( bla = getArgs( "cls='ok\\\\', bli='bla'" ) );
  assertTrue( bla["cls"] == "ok\\" );
  assertTrue( bla["bli"] == "bla" );
}


void test1() {
  startTestSerie( " Test lezen van een FoLiA file " );
  Document d;
  assertNoThrow( d.readFromFile( "tests/folia.example" ) );
}


void test2() {
  startTestSerie( " Test lezen van een FoLiA string " );
  string s;
  ifstream is( "tests/folia.example" );
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
  assertNoThrow( d.readFromFile( "tests/folia.example" ) );
  string out;
  assertNoThrow( out = d.toXml() );
}

void test5() {
  startTestSerie( " Test lezen van een FoLiA file met namespaces" );
  Document d;
  assertNoThrow( d.readFromFile( "tests/folia.nsexample" ) );
  assertNoThrow( d.save( "/tmp/test5.out", "fl", false ) );
  int stat = system( "xmldiff /tmp/test5.out tests/folia.nsexample" );
  assertMessage( "/tmp/test5.out tests/folia.nsexample differ!",
   		 (stat == 0) );
}

Document sanityDoc( "file='tests/folia.example'" );
 
void sanity_test000( ){
  startTestSerie( " Text count " );
  assertEqual( sanityDoc.size(), 1 );
  assertTrue( sanityDoc[0]->isinstance<Text>() );
  
}

void sanity_test001( ){
  startTestSerie(" Paragraph count " );
  assertTrue( sanityDoc.paragraphs().size() == 1 );
  
}

void sanity_test002(){
  startTestSerie(" Sentence count " );
  vector<Sentence*> v;
  assertNoThrow( v = sanityDoc.sentences() );
  assertEqual( size_t(13), v.size() );
  
}

void sanity_test003( ){
  startTestSerie(" Word count " );
  vector<Word*> v;
  assertNoThrow( v = sanityDoc.words() );
  assertEqual( 163, v.size() );
  
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
  assertEqual( w->id(), "WR-P-E-J-0000000001.sandbox.2.s.1.w.6" );
  assertEqual( w->text(), "." );
  assertEqual( str(w), string(".") );
  
}

void sanity_test006( ){
  startTestSerie(" second sentence " );
  FoliaElement* w = 0;
  assertNoThrow( w = sanityDoc.sentences(1) );
  assertTrue( isinstance( w, Sentence_t ) );
  assertTrue( w->id() == "WR-P-E-J-0000000001.p.1.s.1" );
  assertTrue( !w->hastext() );
  assertTrue( w->str() == "Stemma is een ander woord voor stamboom ." );
  
}

void sanity_test007( ){
  startTestSerie(" use index " ); 
  FoliaElement *e = 0;
  assertNoThrow( e = sanityDoc["WR-P-E-J-0000000001.p.1.s.2.w.7"] );
  assertTrue( e->isinstance( Word_t ) );
  assertEqual( e, sanityDoc.index("WR-P-E-J-0000000001.p.1.s.2.w.7") );
  assertEqual( e->id(), string("WR-P-E-J-0000000001.p.1.s.2.w.7") );
  assertTrue( e->text() == "stamboom" );
  
}

void sanity_test008(){
  startTestSerie(" division + head " );
  FoliaElement *e = 0;
  assertNoThrow( e = sanityDoc["WR-P-E-J-0000000001.div0.1"] );
  assertTrue( e->isinstance( Division_t ) );
  FoliaElement *h;
  assertNoThrow( h = e->head() );
  assertEqual( h,  sanityDoc["WR-P-E-J-0000000001.head.1"] );
  assertTrue( len( h ) == 1 );
  
}

void sanity_test009( ){
  startTestSerie(" Token Annotation - Pos " );
  FoliaElement *w = 0;
  assertNoThrow( w = sanityDoc.words(0) );
  assertTrue( w->annotation<PosAnnotation>() == w->select<PosAnnotation>()[0] );
  assertTrue( w->annotation<PosAnnotation>()->isinstance(Pos_t ) );
  //  assertTrue( issubclass( PosAnnotation, AbstractTokenAnnotation ) );
  assertTrue( w->annotation<PosAnnotation>()->cls() == "N(soort,ev,basis,onz,stan)" );
  assertTrue( w->pos() == "N(soort,ev,basis,onz,stan)" );
  assertTrue( w->annotation<PosAnnotation>()->sett() == "cgn-combinedtags" );
  assertTrue( w->annotation<PosAnnotation>()->annotator() == "tadpole" );
  assertTrue( w->annotation<PosAnnotation>()->annotatortype() == AUTO );
  
}

void sanity_test010( ){
  startTestSerie(" Token Annotation - Lemma " );
  FoliaElement *w = 0;
  assertNoThrow( w = sanityDoc.words(0) );
  FoliaElement *l = 0;
  assertNoThrow( l = w->annotation<LemmaAnnotation>() );
  assertTrue( l == w->select<LemmaAnnotation>()[0] );
  assertTrue( isinstance( l, Lemma_t ) );
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
  assertTrue( c->getOriginal()->index(0)->text() == "vierkant" );
  
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

void check( FoliaElement *parent, const string& indent, ostream& os ){
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
	      || parent->isinstance( Semrole_t )
	      || parent->isinstance( Headwords_t )
	      || parent->isinstance( DependencyDependent_t ) ) 
	     && ( child->isinstance( Word_t ) || child->isinstance( Morpheme_t )) ) ){
      assertEqual( parent, child->parent() );
      check( child, indent + " ", os );
    }
  }
}

void sanity_test015( ){
  startTestSerie(" Checking if all elements know who's their daddy " );
  ofstream os( "/tmp/foliaparent.txt" );
  TEST_SILENT_ON();
  check( sanityDoc.doc(), "",  os );
  TEST_SILENT_OFF();
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
  assertThrow( w->description(), NoDescription );
  
}

void sanity_test017(){
  startTestSerie(" Check - Gap " );
  FoliaElement *gap = sanityDoc["WR-P-E-J-0000000001.gap.1"];
  assertEqual( strip(gap->content()).substr(0,11), string("De tekst is") );
  assertTrue( gap->cls() == "backmatter" );
  assertTrue( gap->description() == "Backmatter" );
  
}

void sanity_test018(){
  startTestSerie(" Subtoken annotation (part of speech) " );
  FoliaElement *w = sanityDoc["WR-P-E-J-0000000001.p.1.s.2.w.5"];
  PosAnnotation *m = 0;
  assertNoThrow( m = w->annotation<PosAnnotation>() );
  assertEqual( m->feat("role"),"pv" );
  assertEqual( m->feat("tense"), "tgw" );
  assertEqual( m->feat("form") , "met-t" );
}

void sanity_test019(){
  startTestSerie(" Alignment in same sanityDocument" );
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
  assertTrue( l->index(0)->index(2)->text() == "een ander woord voor stamboom" );
  
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
  assertTrue( context[0] == 0 );
  assertTrue( context[1]->text() == "Stemma" );
  assertTrue( context[2]->text() == "Stemma" );
  
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
  FoliaElement *w = sanityDoc["WR-P-E-J-0000000001.sandbox.2.s.1.w.5"];
  vector<Word*> context = w->rightcontext(3);
  assertTrue( context.size() == 3 );
  assertTrue( text(context[0]) == "." );
  assertTrue( context[1] == 0 );
  assertTrue( context[2] == 0 );
}

void sanity_test024c(){
  startTestSerie(" Obtaining right context with placeholder " );
  FoliaElement *w = sanityDoc["WR-P-E-J-0000000001.sandbox.2.s.1.w.5"];
  vector<Word*> context = w->rightcontext(3, "_");
  assertTrue( context.size() == 3 );
  assertTrue( text(context[0]) == "." );
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
  vector<Word*> context = w->context(150);
  assertTrue( context.size() == 301 );
  assertTrue( context[0] == 0 );
  assertTrue( text(context[202]) == "handschrift" );
  assertTrue( context[300] == 0 );
  
}

void sanity_test025c(){
  startTestSerie(" Obtaining full context with string placeholder " );
  FoliaElement *w = sanityDoc["WR-P-E-J-0000000001.p.1.s.2.w.7"];
  vector<Word*> context = w->context(150, "nil");
  assertTrue( context.size() == 301 );
  assertTrue( text(context[0]) == "nil" );
  assertTrue( text(context[202]) == "handschrift" );
  assertTrue( context[300]->isinstance(PlaceHolder_t) );
  
}

void sanity_test026a(){
  startTestSerie(" Features " );
  FoliaElement *w = sanityDoc["WR-P-E-J-0000000001.p.1.s.6.w.1"];
  FoliaElement *pos = w->annotation<PosAnnotation>();
  assertTrue( pos->isinstance(Pos_t) );
  assertTrue( pos->cls() == "WW(vd,prenom,zonder)" );
  assertTrue( len(pos) ==  1 );
  vector<Feature*> features = pos->select<Feature>();
  assertTrue( len(features) == 1 );
  assertTrue( isinstance(features[0], Feature_t ) );
  assertTrue( features[0]->subset() == "head" );
  assertTrue( features[0]->cls() == "WW" );
  
}

void sanity_test026b(){
  startTestSerie(" Metric " );
  FoliaElement *p= sanityDoc.paragraphs(0);
  FoliaElement *m = p->annotation<MetricAnnotation>();
  assertTrue( isinstance(m, Metric_t ) );
  assertEqual( m->cls(), "sentenceCount" );
  assertEqual( m->feat("value"), "8" );
}

void sanity_test027(){
  startTestSerie(" Time Stamp " );
  FoliaElement *word = sanityDoc["WR-P-E-J-0000000001.p.1.s.8.w.15"];
  FoliaElement *pos = word->annotation<PosAnnotation>();
  assertTrue( pos->getDateTime() == "2011-07-20T19:00:01" );
  assertTrue( pos->xmlstring() == "<pos xmlns=\"http://ilk.uvt.nl/folia\" class=\"N(soort,ev,basis,zijd,stan)\" datetime=\"2011-07-20T19:00:01\"/>" );      
  
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
  FoliaElement *s = sanityDoc["WR-P-E-J-0000000001.p.1.s.6"];
  assertTrue( s->text() == "Verdwenen handschriften waarvan men toch vermoedt dat ze ooit bestaan hebben worden ook in het stemma opgenomen en worden weergegeven door de laatste letters van het alfabet en worden tussen vierkante haken geplaatst ." );
  assertThrow( s->stricttext(), NoSuchText );
  assertTrue( s->stricttext("original") == "Verdwenen handschriften waarvan men toch vermoedt dat ze ooit bestaan hebben worden ook in het stemma opgenomen en worden weergegeven door de laatste letters van het alfabet en worden tussen vierkant haken geplaatst ." );
  assertThrow( s->text( "BLAH" ), NoSuchText );

  FoliaElement *w = sanityDoc["WR-P-E-J-0000000001.p.1.s.4.w.2"];
  assertTrue( w->text() == "hoofdletter" );
  assertTrue( w->textcontent()->text() == "hoofdletter" );
  assertTrue( w->textcontent()->offset() == 3 );
  
  FoliaElement *w2= sanityDoc["WR-P-E-J-0000000001.p.1.s.6.w.31"];
  assertTrue( w2->text() == "vierkante" );
  assertTrue( w2->stricttext() == "vierkante" );
  assertTrue( w2->stricttext("original") == "vierkant" );
  
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
  FoliaElement *l = sanityDoc["sandbox.list.1"];
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
  assertTrue( l->rindex(0)->n() == "2" );
  assertTrue( l->rindex(0)->text() == "Tweede testitem" );
  
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
"xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example\" generator=\"libfolia-v0.8\" version=\"0.8\">\n"
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
"                <pos class=\"NN(blah)\">\n"
"                    <headfeature class=\"NN\" />\n"
"                </pos>\n"
"            </w>\n"
"        </s>\n"
"    </p>\n"
"    <p xml:id=\"p.2\">\n"
"        <s xml:id=\"p.2.s.1\">\n"
"            <w xml:id=\"p.2.s.1.w.1\">\n"
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
  assertTrue( doc["p.1.s.1.w.1"]->pos() == "NN(blah)" );
  assertTrue( doc["p.1.s.1.w.1"]->annotation<PosAnnotation>()->feat("head") == "NN" );
  assertTrue( doc["p.2.s.1.w.1"]->pos() == "BB(blah)" );
  assertTrue( doc["p.2.s.1.w.1"]->annotation<PosAnnotation>()->feat("head") == "BB" );
  
}

void sanity_test037b( ){
  startTestSerie(" Multiclass Feature " );
  string xml = "<?xml version=\"1.0\"?>\n"
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\""
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
  assertTrue( v[0] == "a" );
  assertTrue( v[1] == "b" );
  assertTrue( v[2] == "c" );
  
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
  int stat = system( "xmldiff /tmp/savetest.xml tests/folia.example" );
  assertMessage( "/tmp/savetest.xml tests/folia.example differ!",
		 stat == 0 );
  
}

void sanity_test101( ){
  startTestSerie(" Add a sentence at wrong position " );
  FoliaElement *p = sanityDoc["WR-P-E-J-0000000001.p.1.s.2.w.7"];
  FoliaElement *s = 0;
  assertNoThrow( s = new Sentence( &sanityDoc, "generate_id='" + p->id() + "'" ) );
  assertThrow( p->append( s ), ValueError );
  
}

void sanity_test101a(){
  startTestSerie(" Metadata external reference (CMDI) " );
  Document doc = Document("file='tests/folia.cmdi.xml'");
  assertTrue( doc.metadatatype() == CMDI );
  assertTrue( doc.metadatafile() == "test.cmdi.xml" );
  
}

void sanity_test101b(){
  startTestSerie(" Metadata external reference (IMDI) " );
  Document doc = Document("file='tests/folia.imdi.xml'");
  assertTrue( doc.metadatatype() == IMDI );
  assertTrue( doc.metadatafile() == "test.imdi.xml" );
  
}

void sanity_test102( ){
  startTestSerie(" Add a word at wrong position " );
  FoliaElement *p = sanityDoc["WR-P-E-J-0000000001.p.1.s.2.w.7"];
  assertThrow( p->addWord("text='Ahoi'" ), ValueError );
  
}

void sanity_test102a(){
  startTestSerie(" Declarations - Default set " );
  string xml = "<?xml version=\"1.0\"?>\n"
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\""
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
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\""
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
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\""
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
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\""
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
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\""
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
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\""
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
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\""
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
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\""
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
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\""
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
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\""
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
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\""
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
  KWargs args = getArgs( "set='gap1-set', cls='Y', annotator='proycon'" );
  FoliaElement *g = 0;
  assertNoThrow( g = new Gap( &doc, args ) );
  assertNoThrow( text->append( g ) );
  args = getArgs( "set='gap1-set', cls='Z1'" );
  assertNoThrow( g = new Gap( &doc, args ) );
  assertNoThrow( text->append( g ) );
  args = getArgs( "set='gap2-set', cls='Z2'" );
  assertNoThrow( g = new Gap( &doc, args ) );
  assertNoThrow( text->append( g ) );
  args = getArgs( "set='gap2-set', cls='Y2', annotator='onbekend'" );
  assertNoThrow( g = new Gap( &doc, args ) );
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
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\""
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
  KWargs args = getArgs( "set='other-set', cls='Y', annotator='proycon'" );
  FoliaElement *g = 0;
  assertNoThrow( g = new Gap( &doc, args ) );
  text->append( g );
  args = getArgs( "set='other-set', cls='Z'" );
  assertNoThrow( g = new Gap( &doc, args ) );
  text->append( g );
  vector<Gap*> v = text->select<Gap>();
  assertTrue( v[0]->xmlstring() == "<gap xmlns=\"http://ilk.uvt.nl/folia\" class=\"X\" set=\"gap-set\"/>" );
  assertTrue( v[1]->xmlstring() == "<gap xmlns=\"http://ilk.uvt.nl/folia\" class=\"Y\" set=\"other-set\"/>" );
  assertTrue( v[2]->xmlstring() == "<gap xmlns=\"http://ilk.uvt.nl/folia\" class=\"Z\" set=\"other-set\"/>" );
  
}

void sanity_test102k(){
  startTestSerie(" Declarations - Several annotator types." );
  string xml = "<?xml version=\"1.0\"?>\n"
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\""
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
  KWargs args = getArgs( "set='gap-set', cls='Y', annotatortype='unknown'" );
  FoliaElement *g = 0;
  assertThrow( g = new Gap( &doc, args ), ValueError );
  args = getArgs( "set='gap-set', cls='Y', annotatortype='manual'" );
  assertNoThrow( g = new Gap( &doc, args ) );
  text->append( g );
  args = getArgs( "set='gap-set', cls='Z', annotatortype='auto'" );
  assertNoThrow( g = new Gap( &doc, args ) );
  text->append( g );
  v = text->select<Gap>();
  assertTrue( v[0]->xmlstring() == "<gap xmlns=\"http://ilk.uvt.nl/folia\" annotatortype=\"auto\" class=\"X\" set=\"gap-set\"/>" );
  assertTrue( v[1]->xmlstring() == "<gap xmlns=\"http://ilk.uvt.nl/folia\" annotatortype=\"manual\" class=\"Y\" set=\"gap-set\"/>" );
  assertTrue( v[2]->xmlstring() == "<gap xmlns=\"http://ilk.uvt.nl/folia\" annotatortype=\"auto\" class=\"Z\" set=\"gap-set\"/>" );
  
}

void sanity_test102l(){
  startTestSerie(" Declarations - Datetime default." );
  string xml = "<?xml version=\"1.0\"?>\n"
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\""
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

void sanity_test103( ){
  startTestSerie(" Alien namespaces - Checking whether properly ignored " );
  string xml = "<?xml version=\"1.0\"?>\n"
    " <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\""
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
    "      <w xml:id=\"example.text.1.s.1.w.1\">\n"
    "          <t>word</t>\n"
    "          <alien:invasion number=\"99999\" />\n"
    "      </w>\n"   
    "    </s>\n"
    "  </text>\n"
    "</FoLiA>\n" ;
  Document doc;
  assertNoThrow( doc.readFromString(xml) );
  assertTrue( len(doc.words()) == 1 ); // second word is in alien namespace
  // not read
  FoliaElement *w = doc["example.text.1.s.1.alienword"];
  assertTrue( w == 0 );   // doesn't exist
  w = doc["example.text.1.s.1.w.1"];
  assertTrue( w->xmlstring() == "<w xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example.text.1.s.1.w.1\"><t>word</t></w>" );
  
}


void sanity_test104( ){
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

void sanity_test105( ){
  startTestSerie("105");
  Document doc;
  assertNoThrow( doc.readFromFile("tests/fg.xml") );
  vector<Sentence *> s = doc.sentenceParts();
  assertTrue( s.size() == 24 );
  vector<Word*> wv = s[18]->wordParts();
  assertTrue( wv[1]->str() == "zegt" );
  assertTrue( wv[6]->str() == "doc.p.1.s.1.quote.1.s.15.quote.1.s.1" );
  
}

void sanity_test106( ){
  Document doc;
  startTestSerie( " Attributes - invalid values " );
  string xml = "<?xml version=\"1.0\"?>\n"
" <FoLiA xmlns:xlink=\"http://www.w3.org/1999/xlink\""
"xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"voorbeeld 1\" generator=\"libfolia-v0.8\" version=\"0.8\">\n"
"</FoLiA>\n" ;
  assertThrow( doc.readFromString(xml), XmlError );
  Sentence *s = 0;
  assertThrow( new Sentence(&doc, "id='dit mag ook niet'" ), XmlError );
  assertThrow( new Sentence(&doc, "id='1.ook.niet'" ), XmlError );
  assertThrow( new Sentence(&doc, "id='dit:ook:niet'" ), XmlError );  
}

void edit_test001a( ){
  startTestSerie( " Add a sentence to the last paragraph ");
  FoliaElement *p = 0;
  // grab last paragraph
  Document editDoc( "file='tests/folia.example'" );
  assertNoThrow( p = editDoc.rparagraphs(0) );
  size_t tmp = p->size();
  // add a sentence
  FoliaElement *s = 0;
  assertNoThrow( s = new Sentence( &editDoc, "generate_id='" + p->id() + "'" ) );
  p->append( s );
  // add words to the sentence
  Word *w = 0;
  assertNoThrow( w = new Word( &editDoc, "text='Dit', annotator='testscript', annotatortype='auto', generate_id='" + s->id() + "'" ) );
  s->append( w );
  assertNoThrow( w = new Word( &editDoc, "text='is', annotator='testscript', annotatortype='AUTO', generate_id='" + s->id() + "'" ) );
  s->append( w );  
  assertNoThrow( w = new Word( &editDoc, "text='een', annotator='testscript', annotatortype='auto', generate_id='" + s->id() + "'" ) );
  s->append( w );  
  assertNoThrow( w = new Word( &editDoc, "text='nieuwe', annotator='testscript', annotatortype='auto', generate_id='" + s->id() + "'" ) );
  s->append( w );  
  assertNoThrow( w = new Word( &editDoc, "text='zin', annotator='testscript', annotatortype='auto', generate_id='" + s->id() + "', space='no'" ) );
  s->append( w );  
  assertNoThrow( w = new Word( &editDoc, "text='.', cls='PUNCTUATION', annotator='testscript', annotatortype='auto', generate_id='" + s->id() + "'" ) );
  s->append( w );  
  for ( int i=0; i < 6; ++i ){
    assertTrue( s->index(i)->id() == s->id() + ".w." + toString(i+1) );
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
  assertTrue( s->toktext() == "Dit is een nieuwe zin ." );
  assertTrue( s->text() == "Dit is een nieuwe zin." );

  // all well?

  assertTrue( s->xmlstring() == "<s xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.9\"><w xml:id=\"WR-P-E-J-0000000001.p.1.s.9.w.1\" annotator=\"testscript\"><t>Dit</t></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.9.w.2\" annotator=\"testscript\"><t>is</t></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.9.w.3\" annotator=\"testscript\"><t>een</t></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.9.w.4\" annotator=\"testscript\"><t>nieuwe</t></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.9.w.5\" annotator=\"testscript\" space=\"no\"><t>zin</t></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.9.w.6\" annotator=\"testscript\" class=\"PUNCTUATION\"><t>.</t></w></s>" );
}


void edit_test001b( ){
  startTestSerie( " Add a sentence to the last paragraph (shortcuts)" );
  FoliaElement *p = 0;
  Document editDoc( "file='tests/folia.example'" );
  // grab last paragraph
  assertNoThrow( p = editDoc.rparagraphs(0) );
  size_t tmp = p->size();
  // add a sentence
  FoliaElement *s = 0;
  assertNoThrow( s = p->addSentence() );
  // add words to the sentence
  KWargs ann;
  ann["text"] = "Dit";
  assertNoThrow( s->addWord( ann ) );
  assertNoThrow( s->addWord( "text='is'" ) );
  ann["text"] = "een";
  assertNoThrow( s->addWord( ann ) );
  ann["text"] = "nieuwe";
  assertNoThrow( s->addWord( ann ) );
  ann["text"] = "zin";
  FoliaElement *w = 0;
  assertNoThrow( w = s->addWord( ann ) );
  FoliaElement *w2 = 0;
  assertNoThrow( w2 = s->addWord( "text='.', cls='PUNCTUATION'" ) );
  
  assertTrue( len(s->words()) == 6 );
  assertTrue( w->text() == "zin" );
  assertTrue( editDoc[w->id()] == w );
  
  assertTrue( w2->text() == "." );

  // adition to paragraph correct?
  assertEqual( p->size(), (tmp+1) );
  assertTrue( p->rindex(0) == s );
  
  // all well?
  assertTrue( s->xmlstring() == "<s xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.9\"><w xml:id=\"WR-P-E-J-0000000001.p.1.s.9.w.1\"><t>Dit</t></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.9.w.2\"><t>is</t></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.9.w.3\"><t>een</t></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.9.w.4\"><t>nieuwe</t></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.9.w.5\"><t>zin</t></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.9.w.6\" class=\"PUNCTUATION\"><t>.</t></w></s>" );

}

void edit_test002( ){
  startTestSerie( " Add an token annotation (pos, lemma) " );
  Document editDoc( "file='tests/folia.example'" );
  // grab a word (naam)
  FoliaElement *w = 0;
  assertNoThrow( w = editDoc["WR-P-E-J-0000000001.p.1.s.2.w.11"] );
  
  assertNoThrow( editDoc.declare( AnnotationType::POS, 
					"adhocpos") );

  assertNoThrow( editDoc.declare( AnnotationType::LEMMA, 
					"adhoclemma") );

  // add a pos annotation (in a different set than the one already present, to prevent conflict)
  KWargs args = getArgs( "set='adhocpos', cls='NOUN', annotator='testscript', annotatortype='auto'" );
  assertNoThrow( w->addPosAnnotation( args ) );
  args = getArgs( "set='adhoclemma', cls='NAAM', annotator='testscript', annotatortype='auto', datetime='1982-12-15T19:00:01'");
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
  assertTrue( w->xmlstring() == "<w xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.2.w.11\"><t>naam</t><pos class=\"N(soort,ev,basis,zijd,stan)\" set=\"cgn-combinedtags\"/><lemma class=\"naam\" set=\"lemmas-nl\"/><pos annotator=\"testscript\" annotatortype=\"auto\" class=\"NOUN\" set=\"adhocpos\"/><lemma annotator=\"testscript\" annotatortype=\"auto\" class=\"NAAM\" datetime=\"1982-12-15T19:00:01\" set=\"adhoclemma\"/></w>");
  
}

void edit_test003( ){
  startTestSerie( " Add an token annotation (pos, lemma) (alternative) " );
  // grab a word (naam)
  Document editDoc( "file='tests/folia.example'" );
  FoliaElement *w = 0;
  assertNoThrow( w = editDoc["WR-P-E-J-0000000001.p.1.s.2.w.11"] );

  assertNoThrow( editDoc.declare( AnnotationType::POS, "adhocpos") );

  assertNoThrow( editDoc.declare( AnnotationType::LEMMA, "adhoclemma") );

  // add a pos annotation (in a different set than the one already present, to prevent conflict)
  KWargs args = getArgs( "set='adhocpos', cls='NOUN', annotator='testscript', annotatortype='auto'" );
  assertNoThrow( w->addAnnotation<PosAnnotation>( args ) );
  args = getArgs( "set='adhoclemma', cls='NAAM', annotator='testscript', annotatortype='auto'" );
  assertNoThrow( w->addAnnotation<LemmaAnnotation>( args ) );

  FoliaElement *p = 0;
  assertNoThrow( p = w->annotation<PosAnnotation>( "adhocpos") );
  assertTrue( p->isinstance<PosAnnotation>() );
  assertTrue( p->cls() == "NOUN" );

  assertNoThrow( p = w->annotation<LemmaAnnotation>( "adhoclemma") );
  assertTrue( p->isinstance<LemmaAnnotation>() );
  assertTrue( p->cls() == "NAAM" );

  // check the outcome
  assertTrue( w->xmlstring() == "<w xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.2.w.11\"><t>naam</t><pos class=\"N(soort,ev,basis,zijd,stan)\" set=\"cgn-combinedtags\"/><lemma class=\"naam\" set=\"lemmas-nl\"/><pos annotator=\"testscript\" annotatortype=\"auto\" class=\"NOUN\" set=\"adhocpos\"/><lemma annotator=\"testscript\" annotatortype=\"auto\" class=\"NAAM\" set=\"adhoclemma\"/></w>");
}

void edit_test003b( ){
  startTestSerie( " Add an token annotation (pos, lemma) with missing declaration ") ;
  Document editDoc( "file='tests/folia.example'" );
  // grab a word (naam)
  FoliaElement *w = 0;
  assertNoThrow( w = editDoc["WR-P-E-J-0000000001.p.1.s.2.w.11"] );

  assertNoThrow( editDoc.declare( AnnotationType::POS, "adhocpos") );

  // add a pos annotation (in a different set than the one declared,
  // to generate a conflict)
  KWargs args = getArgs( "set='adhopcos', cls='NOUN', annotator='testscript', annotatortype='auto'" );
  assertThrow( w->addAnnotation<PosAnnotation>( args ), ValueError );
}

void edit_test004a( ){
  startTestSerie( " Add a token default-set annotation which gives a name clash " );
  Document editDoc( "file='tests/folia.example'" );
  // grab a word (naam)
  FoliaElement *w = 0;
  assertNoThrow( w = editDoc["WR-P-E-J-0000000001.p.1.s.2.w.11"] );
  // add a pos annotation without specifying a set (should take default set), but this will clash with existing tag!
  KWargs args = getArgs( "cls='N', annotator='testscript', annotatortype='auto'" );
  assertThrow( w->addPosAnnotation( args ), DuplicateAnnotationError );
  args = getArgs( "cls='naam', annotator='testscript', annotatortype='auto'" );
  assertThrow( w->addLemmaAnnotation( args ), DuplicateAnnotationError );
}

void edit_test004b( ){
  startTestSerie( " Add a token default-set annotation which gives a name clash " );
  Document editDoc( "file='tests/folia.example'" );
  // grab a word which already has morhological info)
  FoliaElement *w = 0;
  assertNoThrow( w = editDoc["WR-P-E-J-0000000001.p.1.s.3.w.5"] );
  // add morhological information without specifying a set (should take default
  // set), but this will clash with existing tag!
  FoliaElement *l = new MorphologyLayer( &editDoc );
  FoliaElement *m = new Morpheme( &editDoc );
  l->append( m );
  FoliaElement *t = new TextContent( &editDoc, "value='hand', offset='0'");
  m->append( t );
  m = new Morpheme( &editDoc );
  l->append( m );
  t = new TextContent( &editDoc, "value='schrift', offset='4'");
  m->append( t );
  m = new Morpheme( &editDoc );
  l->append( m );
  t = new TextContent( &editDoc, "value='en', offset='11'");
  m->append( t );
  assertThrow( w->append( l ), DuplicateAnnotationError );
}

void edit_test005a( ){
  startTestSerie( " Adding an alternative token annotation " );
  Document doc( "file='tests/folia.example'" );
  FoliaElement *w = doc["WR-P-E-J-0000000001.p.1.s.2.w.11"];
  KWargs args = getArgs( "cls='V'" );
  assertNoThrow( w->addAlternative<PosAnnotation>( args ) );
  vector<Alternative*> alt = w->alternatives(); // all alternatives
  string sett = doc.defaultset(AnnotationType::POS);
  vector<Alternative*> alt2 = w->alternatives(sett);
  assertTrue( alt.size() == 1 );
  assertTrue( alt2.size() == 1 );
  assertTrue( alt[0] == alt2[0] );
  FoliaElement *p;
  assertTrue( p = w->annotation<PosAnnotation>( sett ) );
  assertTrue( p->isinstance<PosAnnotation>() );

  std::vector<Alternative *> alt3;
  assertNoThrow( alt3 = w->alternatives(Pos_t, sett) );
  assertTrue( alt3.size() == 1 );
  assertTrue( alt[0] == alt3[0] );

  assertTrue( w->xmlstring() == "<w xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.2.w.11\"><t>naam</t><pos class=\"N(soort,ev,basis,zijd,stan)\"/><lemma class=\"naam\"/><alt xml:id=\"WR-P-E-J-0000000001.p.1.s.2.w.11.alt.1\" auth=\"no\"><pos class=\"V\"/></alt></w>" );

}

void edit_test005b( ){
  startTestSerie( " Adding a morpology layer in a different set" );
  FoliaElement *w = 0;
  Document editDoc( "file='tests/folia.example'" );
  assertNoThrow( w = editDoc["WR-P-E-J-0000000001.p.1.s.3.w.5"] );
  // add morhological information without specifying a set (should take default
  // set), but this will clash with existing tag!
  
  MorphologyLayer *l = 0;
  assertNoThrow( l = w->addAlternative<MorphologyLayer>() );
  FoliaElement *m = new Morpheme( &editDoc );
  l->append( m );
  FoliaElement *t = new TextContent( &editDoc, "value='hand', offset='0'");
  m->append( t );
  m = new Morpheme( &editDoc );
  l->append( m );
  t = new TextContent( &editDoc, "value='schrift', offset='4'");
  m->append( t );
  m = new Morpheme( &editDoc );
  l->append( m );
  t = new TextContent( &editDoc, "value='en', offset='11'");
  m->append( t );
}

void edit_test005c( ){
  startTestSerie( " Adding TextContents in different classes" );
  FoliaElement *s = 0;
  Document editDoc( "file='tests/folia.example'" );
  assertNoThrow( s = editDoc["WR-P-E-J-0000000001.p.1.s.1"] );
  FoliaElement *t = new TextContent( &editDoc, "value='text1', class='test1'");
  assertNoThrow( s->append( t ) );
  t = new TextContent( &editDoc, "value='text1', class='test2'");
  assertNoThrow( s->append( t ) );
}

void edit_test005d( ){
  startTestSerie( " Adding String tags in several classes" );
  FoliaElement *s = 0;
  Document editDoc( "file='tests/folia.example'" );
  assertNoThrow( editDoc.declare( AnnotationType::STRING, 
				  "stringtypes") );
  assertNoThrow( s = editDoc["WR-P-E-J-0000000001.p.1.s.1"] );
  FoliaElement *t = new String( &editDoc, "class='test1'");
  assertNoThrow( t->settext( "text1", "test1" ) );
  assertNoThrow( s->append( t ) );
  t = new String( &editDoc, "class='test2'");
  assertNoThrow( s->append( t ) );
  assertNoThrow( t->settext( "text2" ) );
  assertNoThrow( t->settext( "text2" , "test2" ) );
  assertTrue( t->xmlstring() == "<str xmlns=\"http://ilk.uvt.nl/folia\" class=\"test2\"><t>text2</t><t class=\"test2\">text2</t></str>" );
}

void edit_test006( ){
  startTestSerie( " Correcting text " );
  FoliaElement *w = 0;
  Document doc( "file='tests/folia.example'" );
  assertNoThrow( w = doc["WR-P-E-J-0000000001.p.1.s.8.w.11"] ); // stippelijn
  assertNoThrow( w->correct("new='stippellijn', set='corrections', cls='spelling',annotator='testscript', annotatortype='auto'"  ) );
  FoliaElement *c = 0;
  assertNoThrow( c = w->annotation<Correction>() );
  assertTrue( c->getOriginal()->index(0)->text() == "stippelijn" );
  assertTrue( c->getNew()->index(0)->text() == "stippellijn" );
  assertTrue( w->text() == "stippellijn" );

  assertTrue( w->xmlstring() == "<w xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.11\"><correction xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.11.correction.1\" annotator=\"testscript\" annotatortype=\"auto\" class=\"spelling\"><new><t>stippellijn</t></new><original auth=\"no\"><t>stippelijn</t></original></correction><pos class=\"FOUTN(soort,ev,basis,zijd,stan)\"/><lemma class=\"stippelijn\"/></w>" );
 
}

void edit_test007( ){
  startTestSerie( " Correcting Token Annotation " );
  Document doc( "file='tests/folia.example'" );
  FoliaElement *w = 0;
  assertNoThrow( w = doc["WR-P-E-J-0000000001.p.1.s.8.w.11"] ); // alweer stippelijn
  FoliaElement *oldpos = w->annotation<PosAnnotation>();
  FoliaElement *newpos = new PosAnnotation( &doc, "cls='N(soort,ev,basis,zijd,stan)'" );
  KWargs args = getArgs( "set='corrections', cls='spelling', annotator='testscript', annotatortype='auto'" );
  assertNoThrow( w->correct( oldpos, newpos, args ) );
  FoliaElement *c = 0;
  assertTrue( (c = w->annotation<Correction>()) != 0 );
  assertTrue( c->getOriginal()->index(0) == oldpos );
  assertTrue( (*c->getNew())[0] == newpos );

  assertTrue( w->xmlstring() == "<w xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.11\"><t>stippelijn</t><correction xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.11.correction.1\" annotator=\"testscript\" annotatortype=\"auto\" class=\"spelling\"><new><pos class=\"N(soort,ev,basis,zijd,stan)\"/></new><original auth=\"no\"><pos class=\"FOUTN(soort,ev,basis,zijd,stan)\"/></original></correction><lemma class=\"stippelijn\"/></w>" );
}

void edit_test008( ){
  startTestSerie( " Suggesting a text correction " );
  Document editDoc( "file='tests/folia.example'" );
  FoliaElement *w = editDoc["WR-P-E-J-0000000001.p.1.s.8.w.11"]; // stippelijn
  assertNoThrow( w->correct( "suggestion='stippellijn', set='corrections', cls='spelling',annotator='testscript', annotatortype='auto'" ) );
  FoliaElement *c = 0;
  assertNoThrow( c = w->annotation<Correction>() );
  assertNoThrow( c = c->suggestions()[0] );
  assertTrue( c->text() == "stippellijn" );
  assertTrue( w->xmlstring() == "<w xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.11\"><t>stippelijn</t><pos class=\"FOUTN(soort,ev,basis,zijd,stan)\"/><lemma class=\"stippelijn\"/><correction xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.11.correction.1\" annotator=\"testscript\" annotatortype=\"auto\" class=\"spelling\"><suggestion auth=\"no\"><t>stippellijn</t></suggestion></correction></w>" );        
}

void edit_test009a( ){
  startTestSerie( " Exception on duplicate ID " );
  Document editDoc( "file='tests/folia.example'" );
  FoliaElement *w = 0;
  assertNoThrow( w = editDoc["WR-P-E-J-0000000001.p.1.s.8.w.11"] );
  KWargs kw;
  kw["text"] = "stippellijn";
  kw["id"] = "WR-P-E-J-0000000001.p.1.s.8.w.11";
  assertThrow( w->sentence()->addWord( kw ),
			DuplicateIDError );
}

// void edit_test009b( ){
//   startTestSerie( " Exception on adding TextContent of wrong level " );
//   Document editDoc( "file='tests/folia.example'" );
//   FoliaElement *w = 0;
//   assertNoThrow( w = editDoc["WR-P-E-J-0000000001.p.1.s.8.w.11"] );
//   assertThrow( w->settext( "bla", "original" ), ValueError );
// }

// void edit_test009c( ){
//   startTestSerie( " Exception on adding duplicate TextContent " );
//   Document editDoc( "file='tests/folia.example'" );
//   FoliaElement *w = 0;
//   assertNoThrow( w = editDoc["WR-P-E-J-0000000001.p.1.s.8.w.11"] );
//   TextContent *t = new TextContent( "value='blah', corrected='yes'" );
//   assertThrow( w->append( t ), DuplicateAnnotationError );
// }

void edit_test010( ){
  startTestSerie( " Creating an initially editDocument-less tokenannotation element and adding it to a word " );
  Document editDoc( "file='tests/folia.example'" );
  FoliaElement *w = 0;
  assertNoThrow( w = editDoc["WR-P-E-J-0000000001.p.1.s.8.w.11"] );
  FoliaElement *pos = 0;
  assertThrow( pos = new PosAnnotation( &editDoc, "set='fakecgn', cls='N'" ), ValueError );
  assertNoThrow( editDoc.declare( AnnotationType::POS, 
					"fakecgn") );
  assertNoThrow( pos = new PosAnnotation( &editDoc, "set='fakecgn', cls='N'" ) );
  assertNoThrow( w->append( pos ) );
  assertTrue( pos == w->annotation<PosAnnotation>("fakecgn") );
  assertTrue( pos->parent() == w );
  assertTrue( pos->doc() == w->doc() );

  assertTrue( w->xmlstring() == "<w xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.11\"><t>stippelijn</t><pos class=\"FOUTN(soort,ev,basis,zijd,stan)\" set=\"cgn-combinedtags\"/><lemma class=\"stippelijn\"/><pos class=\"N\" set=\"fakecgn\"/></w>" );
}

void edit_test011(){
  startTestSerie( " Adding Subtoken annotation (morphological analysis)" );
  Document editDoc( "file='tests/folia.example'" );
  FoliaElement *w = editDoc["WR-P-E-J-0000000001.p.1.s.5.w.3"];
  FoliaElement *l = new MorphologyLayer(0);
  w->append( l );
  FoliaElement *m = new Morpheme( &editDoc );
  l->append( m );
  FoliaElement *t = new TextContent( &editDoc, "value='handschrift', offset='0'");
  m->append( t );
  FoliaElement *f = new Feature( &editDoc, "subset='type', cls='stem'");
  m->append( f );
  f = new Feature( &editDoc, "subset='function', cls='lexical'");
  m->append( f );
  m = new Morpheme( &editDoc );
  l->append( m );
  t = new TextContent( &editDoc, "value='en', offset='11'");
  m->append( t );
  f = new Feature( &editDoc, "subset='type', cls='suffix'");
  m->append( f );
  f = new Feature( &editDoc, "subset='function', cls='plural'" );
  m->append( f );
  assertTrue( len(l) ==  2 );  // 2 morphemes
  assertTrue( isinstance( l->index(0), Morpheme_t ) );
  assertTrue( l->index(0)->text() == "handschrift" );
  assertTrue( l->index(0)->feat("type") == "stem" );
  assertTrue( l->index(0)->feat("function") == "lexical" );
  assertTrue( l->index(1)->text() == "en" );
  assertTrue( l->index(1)->feat("type") == "suffix" );
  assertTrue( l->index(1)->feat("function") == "plural" );
  assertTrue( w->xmlstring() == "<w xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.5.w.3\"><t>handschriften</t><pos class=\"N(soort,mv,basis)\"/><lemma class=\"handschrift\"/><morphology><morpheme><t offset=\"0\">handschrift</t><feat class=\"stem\" subset=\"type\"/><feat class=\"lexical\" subset=\"function\"/></morpheme><morpheme><t offset=\"11\">en</t><feat class=\"suffix\" subset=\"type\"/><feat class=\"plural\" subset=\"function\"/></morpheme></morphology></w>" );
}

void edit_test012(){
  startTestSerie( " Edit Check - Adding Alignment" );
  Document editDoc( "file='tests/folia.example'" );
  FoliaElement *w = editDoc["WR-P-E-J-0000000001.p.1.s.6.w.8"];
  Alignment *a = new Alignment( &editDoc, "cls='coreference'" );
  w->append( a );
  AlignReference *ar = new AlignReference( "id='WR-P-E-J-0000000001.p.1.s.6.w.1', type='w'" );
  a->append(ar);
  assertThrow( ar = new AlignReference( "id='wrong', type='word'" ), XmlError );
  assertThrow( ar = new AlignReference( "id='wrong'" ), XmlError );
  ar = new AlignReference( "id='WR-P-E-J-0000000001.p.1.s.6.w.2', type='w'" );
  a->append(ar);
  assertEqual( a->resolve()[0], editDoc["WR-P-E-J-0000000001.p.1.s.6.w.1"] );
  assertEqual( a->resolve()[1], editDoc["WR-P-E-J-0000000001.p.1.s.6.w.2"] );
  assertTrue( w->xmlstring() == "<w xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.6.w.8\"><t>ze</t><pos class=\"VNW(pers,pron,stan,red,3,mv)\"/><lemma class=\"ze\"/><alignment class=\"coreference\"><aref id=\"WR-P-E-J-0000000001.p.1.s.6.w.1\" type=\"w\"/><aref id=\"WR-P-E-J-0000000001.p.1.s.6.w.2\" type=\"w\"/></alignment></w>" );
}

void edit_test013(){
  startTestSerie( " Adding Span Annotatation (syntax) " );
  Document editDoc( "file='tests/folia.example'" );
  FoliaElement *s = editDoc["WR-P-E-J-0000000001.p.1.s.4"];
  //sentence: 'De hoofdletter A wordt gebruikt voor het originele handschrift .'
  FoliaElement *layer = s->append( new SyntaxLayer(&editDoc) );
  FoliaElement *sent = layer->append( new SyntacticUnit( &editDoc, "cls='s'" ) );
  FoliaElement *np = sent->append( new SyntacticUnit( &editDoc,"cls='np'" ) );
  FoliaElement *su = np->append( new SyntacticUnit( &editDoc,"cls='det'" ) );
  su->append( editDoc["WR-P-E-J-0000000001.p.1.s.4.w.1"] );
  su = np->append( new SyntacticUnit( &editDoc,"cls='n'" ) );
  su->append( editDoc["WR-P-E-J-0000000001.p.1.s.4.w.2"] );
  su = np->append( new SyntacticUnit( &editDoc,"cls='n'" ) );
  su->append( editDoc["WR-P-E-J-0000000001.p.1.s.4.w.3"] );
  FoliaElement *vp = sent->append( new SyntacticUnit( &editDoc,"cls='vp'" ) );
  FoliaElement *vps = vp->append( new SyntacticUnit(&editDoc, "cls='vp'" ) );
  su = vps->append( new SyntacticUnit( &editDoc,"cls='v'" ) );
  su->append( editDoc["WR-P-E-J-0000000001.p.1.s.4.w.4"] );
  su = vps->append( new SyntacticUnit( &editDoc,"cls='participle'" ) );
  su->append( editDoc["WR-P-E-J-0000000001.p.1.s.4.w.5"] );
  FoliaElement *pp = vp->append( new SyntacticUnit( &editDoc,"cls='pp'" ) );
  su = pp->append( new SyntacticUnit( &editDoc,"cls='prep'" ) );
  su->append( editDoc["WR-P-E-J-0000000001.p.1.s.4.w.6"] );
  FoliaElement *nps = pp->append( new SyntacticUnit( &editDoc,"cls='np'" ) );
  su = nps->append( new SyntacticUnit( &editDoc,"cls='det'" ) );
  su->append( editDoc["WR-P-E-J-0000000001.p.1.s.4.w.7"] );
  su = nps->append( new SyntacticUnit( &editDoc,"cls='adj'" ) );
  su->append( editDoc["WR-P-E-J-0000000001.p.1.s.4.w.8"] );
  su = nps->append( new SyntacticUnit( &editDoc,"cls='n'" ) );
  su->append( editDoc["WR-P-E-J-0000000001.p.1.s.4.w.9"] );

  assertTrue( layer->xmlstring() == "<syntax xmlns=\"http://ilk.uvt.nl/folia\"><su class=\"s\"><su class=\"np\"><su class=\"det\"><wref id=\"WR-P-E-J-0000000001.p.1.s.4.w.1\" t=\"De\"/></su><su class=\"n\"><wref id=\"WR-P-E-J-0000000001.p.1.s.4.w.2\" t=\"hoofdletter\"/></su><su class=\"n\"><wref id=\"WR-P-E-J-0000000001.p.1.s.4.w.3\" t=\"A\"/></su></su><su class=\"vp\"><su class=\"vp\"><su class=\"v\"><wref id=\"WR-P-E-J-0000000001.p.1.s.4.w.4\" t=\"wordt\"/></su><su class=\"participle\"><wref id=\"WR-P-E-J-0000000001.p.1.s.4.w.5\" t=\"gebruikt\"/></su></su><su class=\"pp\"><su class=\"prep\"><wref id=\"WR-P-E-J-0000000001.p.1.s.4.w.6\" t=\"voor\"/></su><su class=\"np\"><su class=\"det\"><wref id=\"WR-P-E-J-0000000001.p.1.s.4.w.7\" t=\"het\"/></su><su class=\"adj\"><wref id=\"WR-P-E-J-0000000001.p.1.s.4.w.8\" t=\"originele\"/></su><su class=\"n\"><wref id=\"WR-P-E-J-0000000001.p.1.s.4.w.9\" t=\"handschrift\"/></su></su></su></su></su></syntax>" );
}

void edit_test014() {
  startTestSerie( " Replacing an annotation " );
  Document editDoc( "file='tests/folia.example'" );
  FoliaElement *word = editDoc["WR-P-E-J-0000000001.p.1.s.3.w.14"];
  word->replace( new PosAnnotation( &editDoc, "cls='BOGUS'") );
  assertTrue( len(word->annotations<PosAnnotation>() ) ==  1 );
  assertTrue(  word->annotation<PosAnnotation>()->cls() == "BOGUS" );
  assertTrue( word->xmlstring() == "<w xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.3.w.14\"><t>plaats</t><lemma class=\"plaats\"/><pos class=\"BOGUS\"/></w>" );
}

void edit_test015(){
  startTestSerie( " Removing an annotation " );
  Document editDoc( "file='tests/folia.example'" );
  FoliaElement *word = editDoc["WR-P-E-J-0000000001.p.1.s.3.w.14"];
  word->remove( word->annotation<PosAnnotation>() );
  assertThrow( word->annotations<PosAnnotation>(), NoSuchAnnotation );
  assertTrue( word->xmlstring() == "<w xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.3.w.14\"><t>plaats</t><lemma class=\"plaats\"/></w>" );
}

void edit_test016(){
  startTestSerie( " Time Stamp " );
  Document editDoc( "file='tests/folia.example'" );
  FoliaElement *word = editDoc["WR-P-E-J-0000000001.p.1.s.8.w.16"];
  FoliaElement *pos = word->annotation<PosAnnotation>();
  assertNoThrow( pos->setDateTime( "1982-12-15T19:00:01" ) );

  assertTrue( pos->xmlstring() == "<pos xmlns=\"http://ilk.uvt.nl/folia\" class=\"WW(pv,tgw,met-t)\" datetime=\"1982-12-15T19:00:01\"/>" );      
}

void edit_test017(){
  startTestSerie( " Altering word text" );
  Document editDoc( "file='tests/folia.example'" );
  // Important note: directly altering text is usually bad practise, you'll want to use proper corrections instead.
  FoliaElement *word = editDoc["WR-P-E-J-0000000001.p.1.s.8.w.9"];
  assertTrue( word->text() == "terweil" );
  word->settext("terwijl");
  assertTrue( word->text() == "terwijl" );
  assertTrue( word->xmlstring() == "<w xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.9\"><t>terwijl</t><errordetection class=\"spelling\"/><pos class=\"VG(onder)\"/><lemma class=\"terweil\"/></w>" );      
}

void edit_test018a(){
  startTestSerie( " Altering sentence text (untokenized by definition)" );
  Document editDoc( "file='tests/folia.example'" );
  FoliaElement *s = editDoc["WR-P-E-J-0000000001.p.1.s.1"];
  assertTrue( !s->hastext() ); // No text directly associated
  // but text() can be retrieved from children.
  assertTrue( s->text() == "Stemma is een ander woord voor stamboom ." );

  s->settext( "Hooked on text." );
  assertTrue( s->hastext() ); // Now there IS direct text
  assertTrue( s->text() == "Hooked on text." );
}

void edit_test018b(){
  startTestSerie( " Altering sentence text (untokenized by definition)" );
  Document editDoc( "file='tests/folia.example'" );
  FoliaElement *s = editDoc["WR-P-E-J-0000000001.p.1.s.8"];
  assertTrue( s->text() == "Een volle lijn duidt op een verwantschap , terweil een stippelijn op een onzekere verwantschap duidt ." );
  assertNoThrow( s->settext("Een volle lijn duidt op een verwantschap, terwijl een stippellijn op een onzekere verwantschap duidt." ) );
  assertNoThrow( s->settext("Een volle lijn duidt op een verwantschap, terweil een stippelijn op een onzekere verwantschap duidt.", "original" ) );
  assertTrue( s->text() == "Een volle lijn duidt op een verwantschap, terwijl een stippellijn op een onzekere verwantschap duidt." );
  assertTrue( s->hastext("original") );
  assertTrue( s->text("original") == "Een volle lijn duidt op een verwantschap, terweil een stippelijn op een onzekere verwantschap duidt." );

  assertTrue( s->xmlstring() == "<s xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.8\"><t>Een volle lijn duidt op een verwantschap, terwijl een stippellijn op een onzekere verwantschap duidt.</t><t class=\"original\">Een volle lijn duidt op een verwantschap, terweil een stippelijn op een onzekere verwantschap duidt.</t><w xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.1\"><t>Een</t><pos class=\"LID(onbep,stan,agr)\"/><lemma class=\"een\"/></w><quote xml:id=\"WR-P-E-J-0000000001.p.1.s.8.q.1\"><w xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.2\"><t>volle</t><pos class=\"ADJ(prenom,basis,met-e,stan)\"/><lemma class=\"vol\"/></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.3\"><t>lijn</t><pos class=\"N(soort,ev,basis,zijd,stan)\"/><lemma class=\"lijn\"/></w></quote><w xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.4\"><t>duidt</t><pos class=\"WW(pv,tgw,met-t)\"/><lemma class=\"duiden\"/></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.5\"><t>op</t><pos class=\"VZ(init)\"/><lemma class=\"op\"/></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.6\"><t>een</t><pos class=\"LID(onbep,stan,agr)\"/><lemma class=\"een\"/></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.7\"><t>verwantschap</t><pos class=\"N(soort,ev,basis,zijd,stan)\"/><lemma class=\"verwantschap\"/></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.8\"><t>,</t><pos class=\"LET()\"/><lemma class=\",\"/></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.9\"><t>terweil</t><errordetection class=\"spelling\"/><pos class=\"VG(onder)\"/><lemma class=\"terweil\"/></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.10\"><t>een</t><pos class=\"LID(onbep,stan,agr)\"/><lemma class=\"een\"/></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.11\"><t>stippelijn</t><pos class=\"FOUTN(soort,ev,basis,zijd,stan)\"/><lemma class=\"stippelijn\"/></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.12\"><t>op</t><pos class=\"VZ(init)\"/><lemma class=\"op\"/></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.13\"><t>een</t><pos class=\"LID(onbep,stan,agr)\"/><lemma class=\"een\"/></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.14\"><t>onzekere</t><pos class=\"ADJ(prenom,basis,met-e,stan)\"/><lemma class=\"onzeker\"/><correction xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.14.c.1\" class=\"spelling\"><suggestion auth=\"no\" n=\"1/2\"><t>twijfelachtige</t></suggestion><suggestion auth=\"no\" n=\"2/2\"><t>ongewisse</t></suggestion></correction></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.15\"><t>verwantschap</t><pos class=\"N(soort,ev,basis,zijd,stan)\" datetime=\"2011-07-20T19:00:01\"/><lemma class=\"verwantschap\"/></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.16\"><t>duidt</t><pos class=\"WW(pv,tgw,met-t)\"/><lemma class=\"duiden\"/></w><w xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.17\"><t>.</t><pos class=\"LET()\"/><lemma class=\".\"/></w></s>" );
}

void edit_test019(){
  startTestSerie( " Error Detection " );
  Document editDoc( "file='tests/folia.example'" );
  FoliaElement *word = editDoc["WR-P-E-J-0000000001.p.1.s.8.w.11"]; // stippelijn
  word->append( new ErrorDetection(&editDoc,"cls='spelling', annotator='testscript', annotatortype='auto'" ) );
  assertTrue( word->annotation<ErrorDetection>()->cls() == "spelling" );
}


void create_test001( ){
  startTestSerie( " Creating a document from scratch. " );
  Document d( "id='example'" );
  assertNoThrow( d.declare( AnnotationType::TOKEN, 
				      "adhocset", 
				      "annotator='proycon'" ) );
  assertTrue( d.defaultset(AnnotationType::TOKEN) == "adhocset" );
  assertTrue( d.defaultannotator(AnnotationType::TOKEN) == "proycon" );
  string id = d.id() + ".text.1";
  FoliaElement *text = 0;
  KWargs kw = getArgs( "id='" + id + "'" );
  assertNoThrow( text = d.addNode( Text_t, kw ) );
  kw.clear();
  FoliaElement *s = 0;
  assertNoThrow( s = new Sentence( &d, "generate_id='" + text->id() + "'" ) );
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
  //  assertNoThrow( d.save( "/tmp/foliacreatetest001.xml" ) );
  assertTrue( d[id+".s.1"]->size() == 5 );
}

void create_test002( ){
  startTestSerie( " Creating a document from scratch. " );
  Document d( "id='example'" );
  assertNoThrow( d.declare( AnnotationType::POS, 
				      "adhocset", 
				      "annotator='proycon'" ) );
  assertNoThrow( d.declare( AnnotationType::POS, 
				      "myset", 
				      "annotator='sloot'" ) );
  assertTrue( d.defaultset(AnnotationType::POS) == "" );
  assertTrue( d.defaultannotator(AnnotationType::POS) == "" );
  assertTrue( d.defaultannotator(AnnotationType::POS, "myset") == "sloot" );
  string id = d.id() + ".text.1";
  FoliaElement *text = 0;
  KWargs kw = getArgs( "id='" + id + "'" );
  assertNoThrow( text = d.addNode( Text_t, kw ) );
  kw.clear();
  FoliaElement *s = 0;
  assertNoThrow( s = new Sentence( &d, "generate_id='" + text->id() + "'" ) );
  text->append( s );
  kw.clear();
  kw["text"] = "landen";
  FoliaElement *w = 0;
  assertNoThrow( w = s->addWord( kw ) );
  kw.clear();
  kw["set"] = "myset";
  kw["cls"] = "NP";
  assertNoThrow( w->addAnnotation<PosAnnotation>( kw ) );
  kw["cls"] = "VP";
  kw["set"] = "adhocset";
  assertNoThrow( w->addAnnotation<PosAnnotation>( kw ) );
  vector<PosAnnotation*> v = w->select<PosAnnotation>( "adhocset" );
  assertTrue( v.size() == 1 );
  vector<PosAnnotation*> v1 = w->select<PosAnnotation>( "myset" );
  assertTrue( v1.size() == 1 );
  vector<PosAnnotation*> v2 = w->select<PosAnnotation>( "noset" );
  assertTrue( v2.size() == 0 ); 
  vector<PosAnnotation*> v3 = w->select<PosAnnotation>();
  assertTrue( v3.size() == 2 ); 
  assertNoThrow( d.save( "/tmp/foliacreatetest002.xml" ) );
  assertTrue( w->xmlstring() == "<w xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example.text.1.s.1.w.1\"><t>landen</t><pos class=\"NP\" set=\"myset\"/><pos class=\"VP\" set=\"adhocset\"/></w>" );
}

void create_test003( ){
  startTestSerie( " Creating a document with gap annotations from scratch. " );
  Document d( "id='example'" );
  assertNoThrow( d.declare( AnnotationType::GAP, 
				      "gap-set",
				      "annotator='sloot'" ) );
  assertNoThrow( d.declare( AnnotationType::GAP, 
				      "extended-gap-set", 
				      "annotator='sloot'" ) );
  assertTrue( d.defaultset(AnnotationType::GAP) == "" );
  assertTrue( d.defaultannotator(AnnotationType::GAP) == "" );
  assertTrue( d.defaultannotator(AnnotationType::GAP, "gap-set") == "sloot" );
  assertTrue( d.defaultannotator(AnnotationType::GAP, "extended-gap-set") == "sloot" );
  string id = d.id() + ".text.1";
  FoliaElement *text = 0;
  KWargs kw = getArgs( "id='" + id + "'" );
  assertNoThrow( text = d.addNode( Text_t, kw ) );
  kw.clear();
  kw["set"] = "gap-set";
  kw["cls"] = "NP";
  FoliaElement *g = 0;
  assertNoThrow( g = new Gap( &d, kw ) );
  text->append( g );
  kw.clear();
  kw["set"] = "extended-gap-set";
  kw["cls"] = "VP";
  assertNoThrow( g = new Gap( &d, kw ) );
  text->append( g );
  vector<Gap*> v = text->select<Gap>( "gap-set" );
  assertTrue( v.size() == 1 );
  vector<Gap*> v1 = text->select<Gap>( "extended-gap-set" );
  assertTrue( v1.size() == 1 );
  vector<Gap*> v3 = text->select<Gap>();
  assertTrue( v3.size() == 2 ); 
  assertTrue( text->xmlstring() == "<text xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example.text.1\"><gap class=\"NP\" set=\"gap-set\"/><gap class=\"VP\" set=\"extended-gap-set\"/></text>" );
}

void correction_test001a( ){
  startTestSerie( " Split correction " );
  Document *corDoc = new Document( "id='example'" );
  assertNoThrow( corDoc->declare( AnnotationType::TOKEN, 
				  "adhocset", 
				  "annotator='proycon'" ) );
  Text *text = new Text( "id='" + corDoc->id() + ".text.1'" );
  assertNoThrow( corDoc->append( text ) );
  FoliaElement *s = text->append( new Sentence( "id='" + corDoc->id() + ".s.1'"  ) );
  s->append( new Word( corDoc, "text='De', id='" + corDoc->id() + ".s.1.w.1'" ) );
  s->append( new Word( corDoc, "text='site', id='" + corDoc->id() + ".s.1.w.2'" ) );
  s->append( new Word( corDoc, "text='staat', id='" + corDoc->id() + ".s.1.w.3'" ) );
  s->append( new Word( corDoc, "text='online', id='" + corDoc->id() + ".s.1.w.4'" ) );
  s->append( new Word( corDoc, "text='.', id='" + corDoc->id() + ".s.1.w.5'" ) );
  FoliaElement *w = corDoc->index(corDoc->id() + ".s.1.w.4");
  w->split( new Word( corDoc, "id='" + corDoc->id() + ".s.1.w.4a', text='on'" ),
   	    new Word( corDoc, "id='" + corDoc->id() + ".s.1.w.4b', text='line'" ) );
  //  assertNoThrow( corDoc->save( "/tmp/foliasplit1a.xml" ) );
  s = corDoc->index("example.s.1");
  assertTrue( s->rwords(2)->text() == "on" );
  assertTrue( s->rwords(1)->text() == "line" );
  assertTrue( s->text() == "De site staat on line ." );
  assertTrue( len( s->words() ) == 6 );
  assertTrue( s->xmlstring() ==  "<s xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example.s.1\"><w xml:id=\"example.s.1.w.1\"><t>De</t></w><w xml:id=\"example.s.1.w.2\"><t>site</t></w><w xml:id=\"example.s.1.w.3\"><t>staat</t></w><correction xml:id=\"example.s.1.correction.1\"><new><w xml:id=\"example.s.1.w.4a\"><t>on</t></w><w xml:id=\"example.s.1.w.4b\"><t>line</t></w></new><original auth=\"no\"><w xml:id=\"example.s.1.w.4\"><t>online</t></w></original></correction><w xml:id=\"example.s.1.w.5\"><t>.</t></w></s>" );
}

void correction_test001b( ){
  startTestSerie( " Split suggestion " );
  Document *corDoc = new Document( "id='example'" );
  assertNoThrow( corDoc->declare( AnnotationType::TOKEN, 
				  "adhocset", 
				  "annotator='proycon'" ) );
  Text *text = new Text( "id='" + corDoc->id() + ".text.1'" );
  assertNoThrow( corDoc->append( text ) );
  FoliaElement *s = text->append( new Sentence( "id='" + corDoc->id() + ".s.1'"  ) );
  s->append( new Word( corDoc, "text='De', id='" + corDoc->id() + ".s.1.w.1'" ) );
  s->append( new Word( corDoc, "text='site', id='" + corDoc->id() + ".s.1.w.2'" ) );
  s->append( new Word( corDoc, "text='staat', id='" + corDoc->id() + ".s.1.w.3'" ) );
  s->append( new Word( corDoc, "text='online', id='" + corDoc->id() + ".s.1.w.4'" ) );
  s->append( new Word( corDoc, "text='.', id='" + corDoc->id() + ".s.1.w.5'" ) );
  FoliaElement *w = corDoc->index(corDoc->id() + ".s.1.w.4");
  Word *w1 = new Word( corDoc, "generate_id='" + s->id() + "',text='on'" );
  Word *w2 = new Word( corDoc, "generate_id='" + s->id() + "',text='line'" );
  w->split( w1, w2, "suggest='true'" );
  assertNoThrow( corDoc->save( "/tmp/foliasplit1b.xml" ) );
  s = corDoc->index("example.s.1");
  assertTrue( len( s->words() ) == 5 );
  assertTrue( len( corDoc->words() ) == 5 );
  assertTrue( s->rwords(1)->text() == "online" );
  assertTrue( s->text() == "De site staat online ." );
  assertTrue( s->xmlstring() == "<s xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example.s.1\"><w xml:id=\"example.s.1.w.1\"><t>De</t></w><w xml:id=\"example.s.1.w.2\"><t>site</t></w><w xml:id=\"example.s.1.w.3\"><t>staat</t></w><correction xml:id=\"example.s.1.correction.1\"><current><w xml:id=\"example.s.1.w.4\"><t>online</t></w></current><suggestion auth=\"no\"><w xml:id=\"example.s.1.w.6\"><t>on</t></w><w xml:id=\"example.s.1.w.7\"><t>line</t></w></suggestion></correction><w xml:id=\"example.s.1.w.5\"><t>.</t></w></s>");
}
     
void correction_test002(){
  startTestSerie( " Merge corrections " );
  Document *corDoc = new Document( "id='example'" );
  assertNoThrow( corDoc->declare( AnnotationType::TOKEN, 
				  "adhocset", 
				  "annotator='proycon'" ) );
  Text *text = new Text( "id='" + corDoc->id() + ".text.1'" );
  assertNoThrow( corDoc->append( text ) );
  FoliaElement *s = text->append( new Sentence( "id='" + corDoc->id() + ".s.1'"	 ) );
  s->append( new Word( corDoc, "text='De', id='" + corDoc->id() + ".s.1.w.1'" ) );
  s->append( new Word( corDoc, "text='site', id='" + corDoc->id() + ".s.1.w.2'" ) );
  s->append( new Word( corDoc, "text='staat', id='" + corDoc->id() + ".s.1.w.3'" ) );
  s->append( new Word( corDoc, "text='on', id='" + corDoc->id() + ".s.1.w.4'" ) );
  s->append( new Word( corDoc, "text='line', id='" + corDoc->id() + ".s.1.w.5'" ) );
  s->append( new Word( corDoc, "text='.', id='" + corDoc->id() + ".s.1.w.6'" ) );

  vector<FoliaElement *> ow;
  ow.push_back( corDoc->index(corDoc->id() + ".s.1.w.4") );
  ow.push_back( corDoc->index(corDoc->id() + ".s.1.w.5") );
  s->mergewords( new Word( corDoc, "id='" + corDoc->id() + ".s.1.w.4-5', text='online'" ), ow );
  //  assertNoThrow( corDoc->save( "/tmp/foliamerge002.xml" ) );
  assertTrue( len(s->words() ) ==  5 );
  assertTrue( s->text() == "De site staat online ." );
  // incorrection() test, check if newly added word correctly reports being part of a correction
  FoliaElement *w = corDoc->index(corDoc->id() + ".s.1.w.4-5");
  assertTrue( isinstance(w->incorrection(), Correction_t) );
  //incorrection return the correction the word is part of, or None if not part of a correction, 
  assertTrue( s->xmlstring() == "<s xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example.s.1\"><w xml:id=\"example.s.1.w.1\"><t>De</t></w><w xml:id=\"example.s.1.w.2\"><t>site</t></w><w xml:id=\"example.s.1.w.3\"><t>staat</t></w><correction xml:id=\"example.s.1.correction.1\"><new><w xml:id=\"example.s.1.w.4-5\"><t>online</t></w></new><original auth=\"no\"><w xml:id=\"example.s.1.w.4\"><t>on</t></w><w xml:id=\"example.s.1.w.5\"><t>line</t></w></original></correction><w xml:id=\"example.s.1.w.6\"><t>.</t></w></s>" );
}

void correction_test003(){
  startTestSerie( " Delete corrections " );
  Document *corDoc = new Document( "id='example'" );
  assertNoThrow( corDoc->declare( AnnotationType::TOKEN, 
				  "adhocset", 
				  "annotator='proycon'" ) );
  Text *text = new Text( "id='" + corDoc->id() + ".text.1'" );
  assertNoThrow( corDoc->append( text ) );
  FoliaElement *s = text->append( new Sentence( "id='" + corDoc->id() + ".s.1'"	 ) );
  s->append( new Word( corDoc, "text='Ik', id='" + corDoc->id() + ".s.1.w.1'" ) );
  s->append( new Word( corDoc, "text='zie', id='" + corDoc->id() + ".s.1.w.2'" ) );
  s->append( new Word( corDoc, "text='een', id='" + corDoc->id() + ".s.1.w.3'" ) );
  s->append( new Word( corDoc, "text='groot', id='" + corDoc->id() + ".s.1.w.4'" ) );
  s->append( new Word( corDoc, "text='huis', id='" + corDoc->id() + ".s.1.w.5'" ) );
  s->append( new Word( corDoc, "text='.', id='" + corDoc->id() + ".s.1.w.6'" ) );

  s->deleteword( corDoc->index( corDoc->id() + ".s.1.w.4" ) );
  assertNoThrow( corDoc->save( "/tmp/foliadelete003.xml" ) );
  assertTrue( s->text() == "Ik zie een huis ." );
  assertTrue( s->xmlstring() =="<s xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example.s.1\"><w xml:id=\"example.s.1.w.1\"><t>Ik</t></w><w xml:id=\"example.s.1.w.2\"><t>zie</t></w><w xml:id=\"example.s.1.w.3\"><t>een</t></w><correction xml:id=\"example.s.1.correction.1\"><original auth=\"no\"><w xml:id=\"example.s.1.w.4\"><t>groot</t></w></original></correction><w xml:id=\"example.s.1.w.5\"><t>huis</t></w><w xml:id=\"example.s.1.w.6\"><t>.</t></w></s>");
}

void correction_test004(){
  startTestSerie( " Insert corrections " );
  Document *corDoc = new Document( "id='example'" );
  assertNoThrow( corDoc->declare( AnnotationType::TOKEN, 
				  "adhocset", 
				  "annotator='proycon'" ) );
  Text *text = new Text( "id='" + corDoc->id() + ".text.1'" );
  assertNoThrow( corDoc->append( text ) );
  FoliaElement *s = text->append( new Sentence( "id='" + corDoc->id() + ".s.1'"	 ) );
  s->append( new Word( corDoc, "text='Ik', id='" + corDoc->id() + ".s.1.w.1'" ) );
  s->append( new Word( corDoc, "text='zie', id='" + corDoc->id() + ".s.1.w.2'" ) );
  s->append( new Word( corDoc, "text='een', id='" + corDoc->id() + ".s.1.w.3'" ) );
  s->append( new Word( corDoc, "text='huis', id='" + corDoc->id() + ".s.1.w.4'" ) );
  s->append( new Word( corDoc, "text='.', id='" + corDoc->id() + ".s.1.w.5'" ) );

  s->insertword( new Word( corDoc, "id='" + corDoc->id() + ".s.1.w.3b', text='groot'" ), corDoc->index( corDoc->id() + ".s.1.w.3" ) );
  assertNoThrow( corDoc->save( "/tmp/foliainsert004.xml" ) );
  assertTrue( s->text() == "Ik zie een groot huis ." );
  assertTrue( s->xmlstring() == "<s xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"example.s.1\"><w xml:id=\"example.s.1.w.1\"><t>Ik</t></w><w xml:id=\"example.s.1.w.2\"><t>zie</t></w><w xml:id=\"example.s.1.w.3\"><t>een</t></w><correction xml:id=\"example.s.1.correction.1\"><new><w xml:id=\"example.s.1.w.3b\"><t>groot</t></w></new><original auth=\"no\"/></correction><w xml:id=\"example.s.1.w.4\"><t>huis</t></w><w xml:id=\"example.s.1.w.5\"><t>.</t></w></s>" );
}

void correction_test005(){
  startTestSerie( " Re-using a correction with only suggestions " );
  Document *corDoc = new Document();
  corDoc->readFromFile( "tests/folia.example" );
  FoliaElement *w = corDoc->index("WR-P-E-J-0000000001.p.1.s.8.w.11"); // stippelijn
  assertNoThrow( w->correct("suggestion='stippellijn', set='corrections', cls='spelling',annotator='testscript', annotatortype='auto'" ) );
  assertNoThrow( corDoc->save( "/tmp/foliainsert005-1.xml" ) );
  FoliaElement *c = 0;
  assertNoThrow( c = w->annotation<Correction>() );
  assertTrue( c->suggestions()[0]->text() == "stippellijn" );
  assertTrue( w->text() == "stippelijn" );  

  assertNoThrow( w->correct("new='stippellijn', set='corrections', cls='spelling',annotator='John Doe', annotatortype='manual', reuse='" + c->id() + "'" ) );

  assertTrue( w->text() == "stippellijn" );
  assertTrue( len(w->annotations<Correction>()) == 1 );
  assertTrue( w->annotation<Correction>()->suggestions()[0]->text() == "stippellijn" );
  assertTrue( w->annotation<Correction>()->suggestions()[0]->annotator() == "testscript" );
  assertTrue( w->annotation<Correction>()->suggestions()[0]->annotatortype() == AUTO );
  assertTrue( w->annotation<Correction>()->getNew()->text() == "stippellijn" );
  assertTrue( w->annotation<Correction>()->annotator() == "John Doe" );
  assertTrue( w->annotation<Correction>()->annotatortype() == MANUAL );

  assertTrue( w->xmlstring() == "<w xmlns=\"http://ilk.uvt.nl/folia\" xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.11\"><pos class=\"FOUTN(soort,ev,basis,zijd,stan)\"/><lemma class=\"stippelijn\"/><correction xml:id=\"WR-P-E-J-0000000001.p.1.s.8.w.11.correction.1\" annotator=\"John Doe\" class=\"spelling\"><suggestion annotator=\"testscript\" annotatortype=\"auto\" auth=\"no\"><t>stippellijn</t></suggestion><new><t>stippellijn</t></new><original auth=\"no\"><t>stippelijn</t></original></correction></w>" );
}


Document qDoc( "file='tests/folia.example'" );

void query_test001(){
  startTestSerie( " Find Words (simple) " );
  vector<string> words;
  words.push_back( "van" );
  words.push_back( "het" );
  words.push_back( "alfabet" );
  vector<vector<Word*> >matches = qDoc.findwords( Pattern(words) );
  assertTrue( matches.size() == 1 );
  assertTrue( len(matches[0]) == 3 );
  assertTrue( matches[0][0]->text() == "van" );
  assertTrue( matches[0][1]->text() == "het" );
  assertTrue( matches[0][2]->text() == "alfabet" );
}

void query_test002(){
  startTestSerie( " Find Words (with wildcard) ");
  vector<string> words;
  words.push_back( "van" );
  words.push_back( "het" );
  words.push_back( "*:1" );
  vector<vector<Word*> >matches = qDoc.findwords( Pattern(words) );
  assertTrue( matches.size() == 1 );
  assertTrue( len(matches[0]) == 3 );

  assertTrue( matches[0][0]->text() == "van" );
  assertTrue( matches[0][1]->text() == "het" );
  assertTrue( matches[0][2]->text() == "alfabet" );
}

void query_test003(){
  startTestSerie( " Find Words by annotation " );
  vector<string> words;
  words.push_back( "de" );
  words.push_back( "historisch" );
  words.push_back( "wetenschap" );
  words.push_back( "worden" );
  vector<vector<Word*> >matches = qDoc.findwords( Pattern(words,
							 Lemma_t ) );
  assertTrue( matches.size() == 1 );
  assertTrue( len(matches[0]) == 4 );

  assertTrue( matches[0][0]->text() == "de" );
  assertTrue( matches[0][1]->text() == "historische" );
  assertTrue( matches[0][2]->text() == "wetenschap" );
  assertTrue( matches[0][3]->text() == "wordt" );
}

void query_test004(){
  startTestSerie( " Find Words using multiple patterns " );
  vector<string> words;
  words.push_back( "de" );
  words.push_back( "historische" );
  words.push_back( "*" );
  words.push_back( "wordt" );
  Pattern p1(words);
  words.clear();
  words.push_back( "de" );
  words.push_back( "historisch" );
  words.push_back( "wetenschap" );
  words.push_back( "worden" );
  Pattern p2(words,Lemma_t );
  list<Pattern> l;
  l.push_back( p1 );
  l.push_back( p2 );
  vector<vector<Word*> >matches = qDoc.findwords( l ); 
  assertTrue( matches.size() == 1 );
  assertTrue( len(matches[0]) == 4 );

  assertTrue( matches[0][0]->text() == "de" );
  assertTrue( matches[0][1]->text() == "historische" );
  assertTrue( matches[0][2]->text() == "wetenschap" );
  assertTrue( matches[0][3]->text() == "wordt" );
}

void query_test005(){
  startTestSerie( " Find Words that aren't there " );
  vector<string> words;
  words.push_back( "bli" );
  words.push_back( "bla" );
  words.push_back( "blu" );
  vector<vector<Word*> >matches = qDoc.findwords( Pattern(words) );
  assertTrue( matches.size() == 0 );
}

void query_test006(){
  startTestSerie( " Find words with overlap " );
  Document doc( "id='test'" );
  FoliaElement *text = new Text( "id='test.text'" );
  doc.append( text );
  FoliaElement *s = new Sentence( "id='" + doc.id() + ".s.1'" );
  text->append( s );
  s->addWord( "text='a'" );
  s->addWord( "text='a'" );
  s->addWord( "text='a'" );
  s->addWord( "text='A'" ); // test caseinsensitivness too
  s->addWord( "text='b'" );
  s->addWord( "text='a'" );
  s->addWord( "text='a'" );
  vector<string> words;
  words.push_back( "a" );
  words.push_back( "a" );  
  vector<vector<Word*> > matches = doc.findwords( Pattern(words) );
  assertTrue( matches.size() == 4 );
  matches = doc.findwords( Pattern(words,"casesensitive='1'") );
  assertTrue( matches.size() == 3 );
}

void query_test007(){
  startTestSerie( " Find Words with context " );
  vector<string> words;
  words.push_back( "van" );
  words.push_back( "het" );
  words.push_back( "alfabet" );
  vector<vector<Word*> >matches = qDoc.findwords( Pattern(words),
						  "leftcontext='3', rightcontext='3'"); 
  assertTrue( matches.size() == 1 );
  assertTrue( len(matches[0]) == 9 );

  assertTrue( matches[0][0]->text() == "de" );
  assertTrue( matches[0][1]->text() == "laatste" );
  assertTrue( matches[0][2]->text() == "letters" );
  assertTrue( matches[0][3]->text() == "van" );
  assertTrue( matches[0][4]->text() == "het" );
  assertTrue( matches[0][5]->text() == "alfabet" );
  assertTrue( matches[0][6]->text() == "en" );
  assertTrue( matches[0][7]->text() == "worden" );
  assertTrue( matches[0][8]->text() == "tussen" );
}

void query_test008(){
  startTestSerie( " Find Words with 'disjunction' (by using regexp) ");
  vector<string> words;
  words.push_back( "de" );
  words.push_back( "regexp('historische|hedendaagse')" );
  words.push_back( "wetenschap" );
  words.push_back( "wordt" );
  vector<vector<Word*> >matches = qDoc.findwords( Pattern(words) );
  assertTrue( matches.size() == 1 );
  assertTrue( len(matches[0]) == 4 );

  assertTrue( matches[0][0]->text() == "de" );
  assertTrue( matches[0][1]->text() == "historische" );
  assertTrue( matches[0][2]->text() == "wetenschap" );
  assertTrue( matches[0][3]->text() == "wordt" );
}

void query_test009(){
  startTestSerie( " Find Words with regular expressions " );
  vector<string> words;
  words.push_back( "de" );
  words.push_back( "regexp('hist.*')" );
  words.push_back( "regexp('.*schap')" );
  words.push_back( "regexp('w[oae]rdt')" );
  vector<vector<Word*> >matches = qDoc.findwords( Pattern(words) );
  assertTrue( matches.size() == 1 );
  assertTrue( len(matches[0]) == 4 );

  assertTrue( matches[0][0]->text() == "de" );
  assertTrue( matches[0][1]->text() == "historische" );
  assertTrue( matches[0][2]->text() == "wetenschap" );
  assertTrue( matches[0][3]->text() == "wordt" );
}

void query_test010a(){
  startTestSerie( " Find Words with variable wildcard " );
  vector<string> words;
  words.push_back( "de" );
  words.push_back( "laatste" );
  words.push_back( "*" );
  words.push_back( "alfabet" );
  vector<vector<Word*> >matches = qDoc.findwords( Pattern(words) );
  assertTrue( matches.size() == 1 );
  assertTrue( len(matches[0]) == 6 );

  assertTrue( matches[0][0]->text() == "de" );
  assertTrue( matches[0][1]->text() == "laatste" );
  assertTrue( matches[0][2]->text() == "letters" );
  assertTrue( matches[0][3]->text() == "van" );
  assertTrue( matches[0][4]->text() == "het" );
  assertTrue( matches[0][5]->text() == "alfabet" );
}

void query_test010b(){
  startTestSerie( " Find words with wildcard and overlap " );
  Document doc( "id='test'" );
  FoliaElement *text = new Text( "id='test.text'" );
  doc.append( text );
  FoliaElement *s = new Sentence( "id='" + doc.id() + ".s.1'" );
  text->append( s );
  s->addWord( "text='a'" );
  s->addWord( "text='b'" );
  s->addWord( "text='c'" );
  s->addWord( "text='d'" );
  s->addWord( "text='a'" );
  s->addWord( "text='b'" );
  s->addWord( "text='c'" );
  vector<string> words;
  words.push_back( "a" );
  words.push_back( "*" );  
  words.push_back( "c" );  
  vector<vector<Word*> > matches = doc.findwords( Pattern(words) );
  assertTrue( matches.size() == 3 );
}

void query_test011(){
  startTestSerie( " Find Words by non existing annotation " );
  vector<string> words;
  words.push_back( "bla" );
  words.push_back( "bla" );
  words.push_back( "blu" );
  vector<vector<Word*> >matches = qDoc.findwords( Pattern(words,
							 Sense_t ) );
  assertTrue( matches.size() == 0 );
}

int main(){
  test0();
  test1();
  test2();
  test3();
  test4();
  test5();
  sanity_test000();
  sanity_test001();
  sanity_test002();
  sanity_test003();
  sanity_test004();
  sanity_test005();
  sanity_test006();
  sanity_test007();
  sanity_test008();
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
  sanity_test031();
  sanity_test032();
  sanity_test033();
  sanity_test034();
  sanity_test035();
  sanity_test036();
  sanity_test037a();
  sanity_test037b();
  sanity_test099();
  sanity_test100a();
  sanity_test100b();
  sanity_test101();
  sanity_test101a();
  sanity_test101b();
  sanity_test101();
  sanity_test101a();
  sanity_test101b();
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
  sanity_test103();
  sanity_test104();
  sanity_test105();
  sanity_test106();
  edit_test001a();
  edit_test001b();
  edit_test002();
  edit_test003();
  edit_test003b();
  edit_test004a();
  edit_test004b();
  edit_test005a();
  edit_test005b();
  edit_test005c();
  edit_test005d();
  edit_test006();
  edit_test007();
  edit_test008();
  edit_test009a();
  // edit_test009b();
  // edit_test009c();
  edit_test010();
  edit_test011();
  edit_test012();
  edit_test013();
  edit_test014();
  edit_test015();
  edit_test016();
  edit_test017();
  edit_test018a();
  edit_test018b();
  edit_test019();
  create_test001();
  create_test002();
  create_test003();
  correction_test001a();
  correction_test001b();
  correction_test002();
  correction_test003();
  correction_test004();
  correction_test005();
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
  summarize_tests();
}
