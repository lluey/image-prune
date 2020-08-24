 
#include "stats.h"
 
//ver 10(?) guessing out here
 
 
 /* basically what I'm doing here is as follows. I'm setting the array up row by row. For every value in each row, I can
     * easily compute what value the thing is supposed to be by picking the value directly above it and adding the sum of the
     * elements in the current row, up to the current index. So I just do that for the whole 2d vector.
     */
    
 
    //honestly ended up writing a whole ton more than I thought I would, but this feels a lot cleaner than just iterating over the whole thing.
    //it'd be funny if my thing is actually slower though. Don't think that's possible though, assuming my code (and the code I would've written)
    //work in the ways I suspect they do / will. 
 
stats::stats(PNG & im){
 
   
    int sumR = 0;
    int sumG = 0;
    int sumB = 0;
 
    int sum2R = 0;
    int sum2G = 0;
    int sum2B = 0;
 
    vector<long> tempR;
    vector<long> tempG;
    vector<long> tempB;
 
    vector<long> tempSqrR;
    vector<long> tempSqrG;
    vector<long> tempSqrB;
 
    //this initializes the first column of the sumRed etc. vectors.
    for (unsigned i = 0; i < im.height(); i++) {
        RGBAPixel * x = im.getPixel(0, i);
        
        sumR += (long) x->r;
        sumG += (long) x->g;
        sumB += (long) x->b;
 
        sum2R += pow((long) x->r, 2);
        sum2G += pow((long) x->g, 2);
        sum2B += pow((long) x->b, 2);
 
        tempR.push_back(sumR);
        tempG.push_back(sumG); 
        tempB.push_back(sumB); 
 
        tempSqrR.push_back(sum2R);
        tempSqrG.push_back(sum2G); 
        tempSqrB.push_back(sum2B); 
    }
 
    //heresy
    sumRed    .push_back(tempR);
    sumGreen  .push_back(tempG);
    sumBlue   .push_back(tempB);   
 
    sumsqRed  .push_back(tempSqrR);
    sumsqGreen.push_back(tempSqrG);
    sumsqBlue .push_back(tempSqrB);
 
    //-------------------------------------
    //initialization of first column is done. Now i gotta do the other colums.
    //-------------------------------------
 
    for (unsigned i = 1; i < im.width(); i++) {
 
        sumR  = 0;
        sumG  = 0;
        sumB  = 0;
        sum2R = 0;
        sum2G = 0;
        sum2B = 0;
 
        tempR.clear();
        tempG.clear();
        tempB.clear();
        tempSqrR.clear();
        tempSqrG.clear();
        tempSqrB.clear();
 
 
        for (unsigned j = 0; j < im.height(); j++) {
            RGBAPixel * x = im.getPixel(i, j);
        
            sumR += (long) x->r;
            sumG += (long) x->g;
            sumB += (long) x->b;
 
            sum2R += pow((long) x->r, 2);
            sum2G += pow((long) x->g, 2);
            sum2B += pow((long) x->b, 2);
 
            tempR.push_back(sumR + sumRed[i - 1][j]);
            tempG.push_back(sumG + sumGreen[i - 1][j]); 
            tempB.push_back(sumB + sumBlue[i - 1][j]); 
 
            tempSqrR.push_back(sum2R + sumsqRed[i - 1][j]);
            tempSqrG.push_back(sum2G + sumsqGreen[i - 1][j]); 
            tempSqrB.push_back(sum2B + sumsqBlue[i - 1][j]); 
        }
 
        
        sumRed    .push_back(tempR);
        sumGreen  .push_back(tempG);
        sumBlue   .push_back(tempB);   
 
        sumsqRed  .push_back(tempSqrR);
        sumsqGreen.push_back(tempSqrG);
        sumsqBlue .push_back(tempSqrB);
    }
 
 
    // cout << "printed" << endl;
    // for (int i = 0; i < im.height(); i++) {
    //     for (int j = 0; j < im.width(); j++) {
    //         cout << sumRed[j][i];
 
    //     }
    // }
 
}
 
 
 
 
long stats::getSum(char channel, pair<int,int> ul, int dim){
  
    int slToAdd = pow(2, dim) - 1;
    long br, bl, tr, tl;
    br = 0;
    bl = 0;
    tr = 0;
    tl = 0;
 
    int x = ul.first;
    int y = ul.second;
 
    //gotta figure out reference here and in getSumsqr cause this super inefficient.
    vector<vector<long>> result;
 
    if (channel == 'r') {
        result = sumRed;
    } else if (channel == 'g') {
        result = sumGreen;
    } else if (channel == 'b') {
        result = sumBlue;
    } else {
        //lmao the code should certainly fail here soo
        assert(1 == 2);
    }
 
    if (x > 0) {
        bl = result[x - 1][y + slToAdd];
    }
 
    if (y > 0) {
        tr = result[x + slToAdd][y - 1];
    }
 
    if (x > 0 && y > 0) {
        tl = result[x - 1][y - 1];
    }
 
    br = result[x + slToAdd][y + slToAdd];
 
    return br - bl - tr + tl;
    
 
}
 
 
 
 
long stats::getSumSq(char channel, pair<int,int> ul, int dim){
    
    int slToAdd = pow(2, dim) - 1;
    long br, bl, tr, tl;
    br = 0;
    bl = 0;
    tr = 0;
    tl = 0;
 
    int x = ul.first;
    int y = ul.second;
 
    vector<vector<long>> result;
 
    if (channel == 'r') {
        result = sumsqRed;
    } else if (channel == 'g') {
        result = sumsqGreen;
    } else if (channel == 'b') {
        result = sumsqBlue;
    } else {
        //lmao the code should certainly fail here soo
        assert(2 == 3);
    }
 
    if (x > 0) {
        bl = result[x - 1][y + slToAdd];
    }
 
    if (y > 0) {
        tr = result[x + slToAdd][y - 1];
    }
 
    if (x > 0 && y > 0) {
        tl = result[x - 1][y - 1];
    }
 
    br = result[x + slToAdd][y + slToAdd];
 
    return br - bl - tr + tl;
 
}
 
 
 
 
 
long stats::rectArea(int dim) {
    return (long) pow(pow(2, dim), 2); 
 
}
 
 
 
 
 
// given a rectangle, compute its sum of squared deviations from mean, over all color channels.
// see written specification for a description of this function.
double stats::getVar(pair<int,int> ul, int dim){
    
//    int sl = pow(2, dim); 
 
    long red1   = getSum('r', ul, dim);
    long red2   = getSumSq('r', ul, dim);
    long green1 = getSum('g', ul, dim);
    long green2 = getSumSq('g', ul, dim);
    long blue1  = getSum('b', ul, dim);
    long blue2  = getSumSq('b', ul, dim);
 
    long a = rectArea(dim);
 
 
    return red2 -   (double) pow(red1, 2) / a + 
           green2 - (double) pow(green1, 2) / a + 
           blue2 -  (double) pow(blue1, 2) / a;
 
}
 
 
RGBAPixel stats::getAvg(pair<int,int> ul, int dim){
 
    long a = rectArea(dim);
 
    long red   = (long) (getSum('r', ul, dim) / a);
    long green = (long) (getSum('g', ul, dim) / a);
    long blue  = (long) (getSum('b', ul, dim) / a);
 
    RGBAPixel temp = RGBAPixel(red, green, blue);
    return temp;
 
}