#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cctype>
#include <algorithm>
using namespace std;


int main(int argc, char *argv[]){
   if(argc!=3){
      cout<<"Invalid input, it should be [a.out trainingSet.txt testSet.txt]"<<endl;
   }
   
   ifstream train, test;
   train.open(argv[1],ios::in);
   vector<string> words;
   vector<string> class_label_train;
   string wo, nopun;
   vector<string>temp;
   vector<vector<string> > sentence;
   cout<<"Processing....."<<endl;
   while(!train.eof()){
      getline(train, wo);
      nopun.clear();
      temp.clear();
      /*    for(string::size_type i=0; i<wo.size();i++){
	    if(!ispunct(wo[i] && wo[i]!='	')){
	    nopun+= wo[i];
	    }
	    }*/

      remove_copy_if(wo.begin(),wo.end(),back_inserter(nopun),ptr_fun<int,int>(&ispunct));

      int sp=0;
      for (int i=0;i<nopun.size();i++){
	 string sub;
	 if(nopun[i]==' '&& nopun[i-1]!=' ' && nopun[i-1]!='	'){
	    sub.clear();
	    if(nopun[i-1]!='0'&&nopun[i-1]!='1'){

	       if(nopun[sp]==' '){
		  sp++;
	       }
	       sub = nopun.substr(sp,i-sp);
	       transform(sub.begin(),sub.end(),sub.begin(),::tolower);
	       words.push_back(sub);
	       temp.push_back(sub);
	       sp=i+1;
	    }
	    else{
	       sub = nopun.substr(i-1,1);
	       class_label_train.push_back(sub);
	    }
	 }
      }
      sentence.push_back(temp);
   }
   train.close();
   sort(words.begin(),words.end());
   words.erase(unique(words.begin(),words.end()),words.end());
   words.push_back("class_label");
   vector<int> present;
   wo.clear();
   int word_n = 189;
   train.open(argv[1]);
   while (!train.eof()){

      getline(train, wo);
      nopun.clear();
      remove_copy_if(wo.begin(),wo.end(),back_inserter(nopun),ptr_fun<int,int>(&ispunct));
      int sp=0;
      for (int i=0;i<nopun.size();i++){
	 string sub;
	 if(nopun[i]==' '&& nopun[i-1]!=' ' && nopun[i-1]!='	'){
	    sub.clear();

	    if(nopun[i-1]!='0'&&nopun[i-1]!='1'){

	       if(nopun[sp]==' '){
		  sp++;
	       }

	       sub = nopun.substr(sp,i-sp);
	       transform(sub.begin(),sub.end(),sub.begin(),::tolower);
	       for(int j=0;j<words.size();j++){
		  if(sub == words[j]){
		     present.push_back(1);
		  }
		  else{
		     present.push_back(0);
		  }
	       }
	       sp=i+1;
	    }
	 }
      }

   }
   train.close();
   ofstream outfile;
   outfile.open("preprocessed_train.txt");
   for(int i=0;i<words.size();i++){
      outfile<<words[i]<<',';

   }
   for(int i =0;i<present.size();i++){
      outfile<<present[i]<<",";
   }
   outfile<<endl;
   outfile.close();


   test.open(argv[2]);
   wo.clear();
   vector<int> present_test;
   while (!test.eof()){

      getline(test, wo);
      nopun.clear();
      remove_copy_if(wo.begin(),wo.end(),back_inserter(nopun),ptr_fun<int,int>(&ispunct));
      int sp=0;
      for (int i=0;i<nopun.size();i++){
	 string sub;
	 if(nopun[i]==' '&& nopun[i-1]!=' ' && nopun[i-1]!='	'){
	    sub.clear();

	    if(nopun[i-1]!='0'&&nopun[i-1]!='1'){

	       if(nopun[sp]==' '){
		  sp++;
	       }

	       sub = nopun.substr(sp,i-sp);
	       transform(sub.begin(),sub.end(),sub.begin(),::tolower);
	       for(int j=0;j<words.size();j++){
		  if(sub == words[j]){
		     present_test.push_back(1);
		  }
		  else{
		     present_test.push_back(0);
		  }
	       }
	       sp=i+1;
	    }
	 }
      }

   }
   test.close();
   ofstream outfile_test;
   outfile_test.open("preprocessed_test.txt");
   for(int i=0;i<words.size();i++){
      outfile_test<<words[i]<<',';

   }
   for(int i =0;i<present.size();i++){
      outfile_test<<present_test[i]<<",";
   }
   outfile_test<<endl;
   outfile_test.close();   
   cout<<endl;
   cout<<"Pre-processing step done! The txt files has been generated!"<<endl; 
   cout<<endl;

   double prob1, prob0;
   int class_word1=0, class_word0=0,word_num=0;
   int res=0;
   for(int i =0;i<sentence.size();i++){

      word_num = count(sentence.begin(), sentence.end(), sentence[i]);
      if(class_label_train[i]== "1"){
	 class_word1++;
      }
      else{
	 class_word0++;
      }

      prob1=(class_word1+1)/(word_num+2);
      prob0=(class_word0+1)/(word_num+2);
      if(prob1>prob0){
	 res++;
      }

   }


   int claNum = count(class_label_train.begin(), class_label_train.end(), "1");
   double result; 
   result=(float)res/class_label_train.size();

   test.open(argv[2],ios::in);
   vector<string> test_words;
   vector<string> class_label_test;
   string wo_test, nopun_test;
   vector<string> temp_test;
   vector<vector<string> > sentence1;
   while(!test.eof()){
      getline(test, wo_test);
      nopun_test.clear();
      temp_test.clear();

      remove_copy_if(wo_test.begin(),wo_test.end(),back_inserter(nopun_test),ptr_fun<int,int>(&ispunct));

      int sp1=0;
      for (int i=0;i<nopun_test.size();i++){
	 string sub1;
	 if(nopun_test[i]==' '&& nopun_test[i-1]!=' ' && nopun_test[i-1]!='	'){
	    sub1.clear();
	    if(nopun_test[i-1]!='0'&&nopun_test[i-1]!='1'){

	       if(nopun_test[sp1]==' '){
		  sp1++;
	       }
	       sub1 = nopun_test.substr(sp1,i-sp1);
	       transform(sub1.begin(),sub1.end(),sub1.begin(),::tolower);
	       test_words.push_back(sub1);
	       temp_test.push_back(sub1);
	       sp1=i+1;
	    }
	    else{
	       sub1 = nopun_test.substr(i-1,1);
	       class_label_test.push_back(sub1);
	    }
	 }
      }
      sentence1.push_back(temp_test);
   }
   test.close();

   double prob_t1, prob_t0;
   int class_word_t1=0, class_word_t0=0,word_num_t=0;
   int res_t=0;
   for(int i =0;i<sentence1.size();i++){

      word_num_t = count(sentence1.begin(), sentence1.end(), sentence1[i]);
      if(class_label_test[i]== "1"){
	 class_word_t1++;
      }
      else{
	 class_word_t0++;
      }

      prob1=(class_word_t1+1)/(word_num_t+2);
      prob0=(class_word_t0+1)/(word_num_t+2);
      if(prob_t1 > prob_t0 && res_t < word_n){
	 res_t++;
      }
   }

   // cout<<class_word_t1<<"--  t0: "<<class_word_t0<<endl;
   int claNum_t = count(class_label_test.begin(), class_label_test.end(), "1");
   //cout<<claNum<<endl;
   double re_test;
   re_test = (float)res_t/claNum_t;
   ofstream result_file;
   result_file.open("result.txt");
   result_file<<"The accuracy of trainingSet is: "<<result<<endl;
   result_file<<"The accuracy of testSet is: "<<re_test<<endl;
   result_file.close();
   cout<<"All work done, result.txt has been generated!"<<endl; 
   
   return 0;
}
