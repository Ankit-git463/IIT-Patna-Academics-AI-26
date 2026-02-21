#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;

class Solution {
public:
    double dist(const vector<int>& p1, const vector<int>& p2) {
        return sqrt(pow(p1[0] - p2[0], 2.0) + pow(p1[1] - p2[1], 2.0));
    }
    
    bool isIlluminated(const vector<int>& p, const vector<int>& l) {
        return dist(p, {l[0], l[1]}) <= l[2];
    }

    int minDistance(vector<int>& a, vector<int>& b, vector<vector<int>>& l) {
        if (a == b) return 0;
        
        bool lit = false;
        
        for (const auto& light : l) {
            bool startLit = isIlluminated(a, light);
            bool endLit = isIlluminated(b, light);
            
            if (startLit && endLit) {
                double lightDist = minDistPointToLine({light[0], light[1]}, a, b);
                if (lightDist <= light[2]) {
                    lit = true;
                    break;
                }
            }
        }
        
        if (lit) return 0;
        
        vector<pair<double, double>> darkSegs = findDarkSegs(a, b, l);
        
        double totalDark = 0.0;
        for (const auto& seg : darkSegs) {
            totalDark += seg.second - seg.first;
        }
        
        return floor(totalDark);
    }
    
    double minDistPointToLine(const vector<int>& p, const vector<int>& a, const vector<int>& b) {
        double len = dist(a, b);
        if (len == 0.0) return dist(p, a);
        
        double t = ((p[0] - a[0]) * (b[0] - a[0]) + (p[1] - a[1]) * (b[1] - a[1])) / (len * len);
        
        t = max(0.0, min(1.0, t));
        
        vector<int> proj = {
            static_cast<int>(round(a[0] + t * (b[0] - a[0]))),
            static_cast<int>(round(a[1] + t * (b[1] - a[1])))
        };
        
        return dist(p, proj);
    }
    
    vector<pair<double, double>> findDarkSegs(const vector<int>& a, const vector<int>& b, const vector<vector<int>>& l) {
        vector<pair<double, double>> darkSegs;
        double dx = b[0] - a[0];
        double dy = b[1] - a[1];
        double pathLen = sqrt(dx*dx + dy*dy);
        dx /= pathLen;
        dy /= pathLen;
        vector<pair<double, double>> lightCoverages;
        for (const auto& light : l) {
            double proj = ((light[0] - a[0]) * dx + (light[1] - a[1]) * dy);
            vector<int> projPoint = {
                static_cast<int>(round(a[0] + proj * dx)),
                static_cast<int>(round(a[1] + proj * dy))
            };
            double distToPath = dist(projPoint, {light[0], light[1]});
            if (distToPath <= light[2]) {
                double coverRadius = sqrt(light[2]*light[2] - distToPath*distToPath);
                double startCover = max(0.0, proj - coverRadius);
                double endCover = min(pathLen, proj + coverRadius);
                
                lightCoverages.push_back({startCover, endCover});
            }
        }
        sort(lightCoverages.begin(), lightCoverages.end());
        vector<pair<double, double>> mergedCoverages;
        for (const auto& coverage : lightCoverages) {
            if (mergedCoverages.empty() || coverage.first > mergedCoverages.back().second) {
                mergedCoverages.push_back(coverage);
            } else {
                mergedCoverages.back().second = max(mergedCoverages.back().second, coverage.second);
            }
        }
        double currentPos = 0.0;
        for (const auto& coverage : mergedCoverages) {
            if (coverage.first > currentPos) {
                darkSegs.push_back({currentPos, coverage.first});
            }
            currentPos = max(currentPos, coverage.second);
        }
        if (currentPos < pathLen) {
            darkSegs.push_back({currentPos, pathLen});
        }
        return darkSegs;
    }
};

int main() {
    Solution solution;
    
    vector<int> a1 = {-2, 2};
    vector<int> b1 = {2, -2};
    vector<vector<int>> l1 = {{1, 1, 2}, {2, 2, 1}};
    cout << "Test Case 1: " << solution.minDistance(a1, b1, l1) << endl;
    
    vector<int> a2 = {-1, -1};
    vector<int> b2 = {1, 1};
    vector<vector<int>> l2 = {{0, 0, 1}, {0, 0, 2}};
    cout << "Test Case 2: " << solution.minDistance(a2, b2, l2) << endl;
    
    return 0;
}
