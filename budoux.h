#ifndef SAORI_BUDOUX_BUDOUX_H_
#define SAORI_BUDOUX_BUDOUX_H_

#include <string>
#include <vector>

namespace budoux {

bool loadModel(std::string& path);

std::vector<std::string> parse(std::string& sentence, int max_width);

}

#endif // SAORI_BUDOUX_BUDOUX_H_
