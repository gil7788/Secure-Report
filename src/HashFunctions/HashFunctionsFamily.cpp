//
// Created by gil on 26.8.2020.
//

#include <range.h>
#include "HashFunctionsFamily.h"


// Hash function family
double HashFunctionFamily::tests_bit_randomness(int number_of_tests) {
    double average = 0;
    for (int n=0; n < number_of_tests; ++n) {
        auto sampled = sample_random_bit();
        average += sampled;
    }
    average = average / number_of_tests;
    return average;
}

int HashFunctionFamily::sample_random_bit() {
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(0, 1);
    auto sampled_bit = distrib(gen);
    return sampled_bit;
}

void HashFunctionFamily::initialize(int domain_word_size, int range_word_size) {
    _domain_word_size = domain_word_size;
    _range_word_size = range_word_size;
}

fs::path HashFunctionFamily::build_log_file_path() {
    auto hash_function_name_with_underscore = get_function_name();
    std::replace( hash_function_name_with_underscore.begin(), hash_function_name_with_underscore.end(), ' ', '_' );
    string directory_name = hash_function_name_with_underscore + "_run_output";
    string file_name = std::to_string(get_domain_word_length()) + "_" + std::to_string(get_range_word_length());
    auto file_path = fs::path(constants::PROCESSED_FILE_PATH);
    file_path /= directory_name;
    file_path /= file_name;
    return file_path;
}

int HashFunctionFamily::get_domain_size() {
    return pow(2, get_domain_word_length());
}

int HashFunctionFamily::get_range_size() {
    int range = pow(2, get_range_word_length());
    return range;
}

int HashFunctionFamily::get_domain_word_length() {
    return _domain_word_size;
}

int HashFunctionFamily::get_range_word_length() {
    return _range_word_size;
}

string HashFunctionFamily::to_string() {
    ostringstream ss;
    ss << *this;
    return ss.str();
}

int HashFunctionFamily::get_value_in_range(int x) {
    return ((x % get_range_size()) + get_range_size()) % get_range_size();
}

VectorXi HashFunctionFamily::get_evaluated_domain() {
    if(_evaluated_domain.empty()) {
        evaluate_all_domain();
    }
    Eigen::Map<Eigen::VectorXi> result(_evaluated_domain.data(), _evaluated_domain.size());
    return result;
}

void HashFunctionFamily::set_evaluated_domain(vector<int>& evaluated_domain) {
    _evaluated_domain = evaluated_domain;
}

void HashFunctionFamily::set_evaluated_domain(VectorXi& evaluated_domain) {
    std::vector<int> converted_evaluated(evaluated_domain.data(), evaluated_domain.data() + evaluated_domain.size());
    set_evaluated_domain(converted_evaluated);
}

void HashFunctionFamily::clear_evaluated_domain() {
    _evaluated_domain.clear();
}

vector<int> HashFunctionFamily::evaluate_subset(vector<int> &values_indices) {
    vector<int> selected_values;
    for (int i = 0; i < values_indices.size(); ++i) {
        selected_values.push_back(get_evaluated_value(values_indices[i]));
    }
    return selected_values;
}

ostream& operator<<(ostream& os, HashFunctionFamily& hash_function) {
    long domain_size = pow(2, hash_function.get_domain_word_length());
    long range_size = pow(2, hash_function.get_range_word_length());
    long number_of_random_bits = hash_function.get_number_of_random_bits();


    string domain_size_string = "Domain size: " + to_string(domain_size) + "\n";
    string range_size_string = "Range size: " + to_string(range_size) + "\n";
    string number_of_random_bits_string = "Number of random bits: " + to_string(number_of_random_bits) + "\n";

    os << domain_size_string;
    os << range_size_string;
    os << number_of_random_bits_string;

    return os;
}

bool HashFunctionFamily::write_evaluated_domain_to_file() {
//    auto file_path = build_log_file_path();
//    InputOutput io(false, file_path, constants::OUTPUT_LEVELS::REGULAR);
//    bool written_to_file = false;
//
//    auto evaluated_domain = evaluate_all_domain();
//    auto evaluated_domain_string = VectorUtils::to_string(evaluated_domain);
//
//    written_to_file = io.output(evaluated_domain_string, constants::OUTPUT_LEVELS::REGULAR);
//    return written_to_file;
}

int HashFunctionFamily::get_evaluated_value(int i) {
    auto domain = get_evaluated_domain();
    return domain[i];
}

// Trivial hash function family
int TrivialHashFunctionsFamily::get_number_of_random_bits() {
    return get_domain_word_length() * get_range_word_length();
}

int TrivialHashFunctionsFamily::evaluate_value(int x) {
    auto x_binary = VectorUtils::number_to_binary(x, get_domain_word_length());
    auto x_evaluation_binary = x_binary.transpose() * _sampled_matrix;

    auto x_evaluation = VectorUtils::binary_to_number(x_evaluation_binary);
    return x_evaluation;
}

void TrivialHashFunctionsFamily::initialize(int domain_word_size, int range_word_size) {
    HashFunctionFamily::initialize(domain_word_size, range_word_size);
}

void TrivialHashFunctionsFamily::build() {
    _sampled_matrix = MatrixXi::NullaryExpr(get_domain_word_length(), get_range_word_length(), [&](){return sample_random_bit();});
}

void TrivialHashFunctionsFamily::evaluate_all_domain() {
    vector<int> evaluated_domain;
    for (int i = 0; i < get_domain_size(); ++i) {
        evaluated_domain.push_back(evaluate_value(i));
    }
    set_evaluated_domain(evaluated_domain);
}

string TrivialHashFunctionsFamily::get_function_name() {
    string trivial_hash_function_name = "Trivial Hash Function Family";
    return trivial_hash_function_name;
}

void KWiseIndependentHashFunctionFamily::initialize(int domain_word_size, int range_word_size, int k) {
    HashFunctionFamily::initialize(domain_word_size, range_word_size);
    set_independence(k);
}

int KWiseIndependentHashFunctionFamily::get_independence() {
    return _k;
}

void KWiseIndependentHashFunctionFamily::set_independence(int k) {
    _k = k;
}

ostream &operator<<(ostream &os, KWiseIndependentHashFunctionFamily &hash_function) {
    int independence = hash_function.get_independence();
    os << (HashFunctionFamily&) hash_function;
    os << "Independence: " << independence << "\n";
    return os;
}

int PolynomialHashFunctionsFamily::get_number_of_random_bits() {
    return get_domain_word_length() * get_independence();
}

void PolynomialHashFunctionsFamily::initialize(int domain_word_size, int range_word_size, int k) {
    KWiseIndependentHashFunctionFamily::initialize(domain_word_size, range_word_size, k);
}

void PolynomialHashFunctionsFamily::build() {
    sample_polynom();
    build_domain_matrix();
}

void PolynomialHashFunctionsFamily::build_domain_matrix() {
    _domain_matrix = MatrixXi::Zero(get_domain_word_length(), get_domain_size());
    for (int i = 0; i < _domain_matrix.cols(); ++i) {
        auto binary_number = VectorUtils::number_to_binary(i, get_domain_word_length());
        _domain_matrix.col(i) = binary_number;
    }
}

void PolynomialHashFunctionsFamily::sample_polynom() {
    _vandermonde_matrix = build_vandermonde_matrix();
    auto seed = sample_seed();

    _polynom = (seed.transpose() * _vandermonde_matrix).unaryExpr([&](const int x) { return x % get_range_size(); });
}

MatrixXi PolynomialHashFunctionsFamily::build_vandermonde_matrix() {
    auto vandermonde_elements = get_vandermonde_elements(get_independence());
    MatrixXi vandemonde_matrix= MatrixXi::Zero(get_independence(), get_domain_word_length());
    int i;
    int j;

    for ( i = 0; i < get_independence(); i++ ) {
        for ( j = 0; j < get_domain_word_length(); j++ ) {
            if( i == 0 && vandermonde_elements[j] == 0 ) {
                vandemonde_matrix(i, j) = 1;
            }
            else {
                vandemonde_matrix(i, j) = pow(vandermonde_elements[j], i );
            }
        }
    }

    return vandemonde_matrix;
}

vector<int> PolynomialHashFunctionsFamily::get_vandermonde_elements(int number_of_element) {
    vector<int> elements;
    for (int i = 0; i < number_of_element; ++i) {
        elements.push_back(i+1);
    }
    return elements;
}

void PolynomialHashFunctionsFamily::evaluate_all_domain() {

    MatrixXi binary_domain = MatrixXi::Zero(get_domain_word_length(), get_domain_size());
    for(auto i: range(get_domain_size())) {
        binary_domain.col(i) = VectorUtils::number_to_binary(i, get_domain_word_length());
    }
    VectorXi evaluated_domain = (_polynom.transpose() * binary_domain).unaryExpr([&](const int x) { return get_value_in_range(x); });
    set_evaluated_domain(evaluated_domain);
//    vector<int> k_wise_independent_elements(evaluated_domain.data(), evaluated_domain.data() + evaluated_domain.rows() * evaluated_domain.cols());
}

//MatrixXi PolynomialHashFunctionsFamily::evaluate_all_domain_binary() {
//    VectorXi evaluated_domain = evaluate_all_domain();
//    MatrixXi binary_evaluated_domain = MatrixXi::Zero(evaluated_domain.size(), get_range_size());
//    for(int i = 0; i < evaluated_domain.size(); ++i) {
//        auto binary_value = VectorUtils::number_to_binary(evaluated_domain(i), get_range_size());
//        binary_evaluated_domain.col(i) = binary_value;
//    }
//
//    return binary_evaluated_domain;
//}

VectorXi PolynomialHashFunctionsFamily::sample_seed() {
    vector<int> seed;
    VectorXi field_seed = VectorXi::Zero(get_independence());

    for (int i = 0; i < get_number_of_random_bits(); ++i) {
        seed.push_back(sample_random_bit());
    }

    for (int i = 0; i < get_independence(); ++i) {
        vector<int>::const_iterator begin = seed.begin() + get_domain_word_length() * i;
        vector<int>::const_iterator end = seed.begin() + get_domain_word_length() * (i+1);
        vector<long> binary_field_element(begin, end);
        auto field_element = VectorUtils::std_vector_to_number(binary_field_element);
        field_seed(i) = field_element;
    }

    return field_seed;
}

string PolynomialHashFunctionsFamily::get_function_name() {
    string polynomial_hash_function_name = "Polynomial Hash Function Family";
    return polynomial_hash_function_name;
}

int GraduallyIncreasingHashFunctionsFamily::get_number_of_random_bits() {
    int random_bits = 0;
    for(auto& hash_function: _hash_functions) {
        random_bits += hash_function.get_number_of_random_bits();
    }
    return random_bits;
}

void GraduallyIncreasingHashFunctionsFamily::build() {
    build_matrices();
}

void GraduallyIncreasingHashFunctionsFamily::evaluate_all_domain() {
    vector<vector<long>> binary_k_wise_independent_elements;
    for(int i = 0; i < _hash_functions.size(); ++i) {
        auto evaluated_domain = (_hash_functions[i]).get_evaluated_domain();

        for(int j = 0; j < evaluated_domain.size(); ++j) {
            vector<long> binary_number = VectorUtils::number_to_std_vector(evaluated_domain(j),
                    _hash_functions[i].get_range_word_length());
            if(i == 0) {
                binary_k_wise_independent_elements.push_back(binary_number);
            }
            else {
                binary_k_wise_independent_elements[j].insert(binary_k_wise_independent_elements[j].end(),
                                                             binary_number.begin(), binary_number.end());
            }
        }
    }

    vector<int> evaluated_domain;
    for(auto& binary_element: binary_k_wise_independent_elements) {
        auto element = VectorUtils::std_vector_to_number(binary_element);
        evaluated_domain.push_back(element);
    }
    set_evaluated_domain(evaluated_domain);
}

void GraduallyIncreasingHashFunctionsFamily::initialize(int domain_word_length, int range_word_length) {
    assert(range_word_length >= 4);
    // Clear object from previous initialization
    _independence_degrees.clear();
    _domains.clear();
    _domains_word_length.clear();
    _hash_functions.clear();
    clear_evaluated_domain();
    HashFunctionFamily::initialize(domain_word_length, range_word_length);
    initialize_matrices();
}

void GraduallyIncreasingHashFunctionsFamily::initialize_matrices() {
    auto n = get_range_size();
    int n_i = n;
    int l_i = floor(log2(n_i)/4);
    int k_i = ceil(get_range_word_length() / l_i);
    int d = floor(log2(get_range_word_length())/2 + 1);
    _domains.push_back(n_i);
    _domains_word_length.push_back(l_i);
    _independence_degrees.push_back(k_i);

    while(n_i <= get_range_word_length()){
        n_i /= pow(2, l_i);
        l_i = floor(log2(n_i)/4);
        k_i = get_range_word_length() / l_i;
        _domains.push_back(n_i);
        _domains_word_length.push_back(l_i);
        _independence_degrees.push_back(k_i);
    }

    n_i /= pow(2, l_i);
    int word_length_sum = std::accumulate(_domains_word_length.begin(), _domains_word_length.end(), 0);
    int l_d = get_range_word_length() - word_length_sum;
    int k_d =  ceil(get_range_word_length() / log2(get_range_word_length()));
    _domains.push_back(n_i);
    _domains_word_length.push_back(l_d);
    _independence_degrees.push_back(k_d);

    cout << "domains: " << VectorUtils::std_vector_to_string<int>(_domains) << "\n";
    cout << "domains_word_length: " << VectorUtils::std_vector_to_string<int>(_domains_word_length) << "\n";
    cout << "independence: " << VectorUtils::std_vector_to_string<int>(_independence_degrees) << "\n";
}

void GraduallyIncreasingHashFunctionsFamily::build_matrices() {
    for (int i = 0; i < _domains_word_length.size(); ++i) {
        PolynomialHashFunctionsFamily hash_function;
        hash_function.initialize(get_domain_word_length(), _domains_word_length[i],
                                 _independence_degrees[i]);
        hash_function.build();
        _hash_functions.push_back(hash_function);
    }
}

int GraduallyIncreasingHashFunctionsFamily::evaluate_value(int value) {
    return get_evaluated_value(value);
}

string GraduallyIncreasingHashFunctionsFamily::get_function_name() {
    string increasing_hash_function_name = "Increasing Hash Function Family";
    return increasing_hash_function_name;
}

int Tabulation::get_number_of_random_bits() {
    int random_bits = 0;
    for(auto& hash_function: _hash_tables) {
        random_bits += hash_function.get_number_of_random_bits();
    }
    return random_bits;
}

void Tabulation::initialize(int domain_word_size, int range_word_size) {
    HashFunctionFamily::initialize(domain_word_size, range_word_size);
    _number_of_hash_tables = ceil(domain_word_size/range_word_size);

    for(int i = 0; i < _number_of_hash_tables; ++i) {
        TrivialHashFunctionsFamily hash;
        auto tables_domain_size = ceil(get_domain_word_length()/_number_of_hash_tables);
        hash.initialize(tables_domain_size, get_domain_word_length());
        _hash_tables.push_back(hash);
    }
}

void Tabulation::build() {
    for(auto& hash_table: _hash_tables) {
        hash_table.build();
    }
}

void Tabulation::evaluate_all_domain() {
    vector<int> evaluated_domain;

//    TODO un comment
    for (int i = 0; i < get_domain_size(); ++i) {
        auto evaluated_value = VectorUtils::binary_to_number(evaluate(i));
        evaluated_domain.push_back(evaluated_value);
    }
    set_evaluated_domain(evaluated_domain);
}

VectorXi Tabulation::evaluate(int value) {
    vector<long> accumulated(get_word_length(), 1);
    auto binary_number = VectorUtils::number_to_std_vector(value, get_domain_word_length());
    for (int i = 0; i < get_number_of_hash_tables()-1; ++i) {
        vector<long>::iterator number_begin = binary_number.begin() + get_word_length() * i;
        vector<long>::iterator number_end = number_begin + get_word_length();
        vector<long> word_i(number_begin, number_end);
        accumulated = VectorUtils::xor_vectors(accumulated, word_i);
    }

    std::vector<int> acc(begin(accumulated), end(accumulated));
    Eigen::Map<Eigen::VectorXi> result(acc.data(), acc.size());
    return result;
}

int Tabulation::get_number_of_hash_tables() {
    return _number_of_hash_tables;
}

int Tabulation::get_word_length() {
    auto word_length = ceil(get_domain_word_length() / get_number_of_hash_tables());
    return word_length;
}

string Tabulation::get_function_name() {
    string tabulation_hash_function_name = "Tabulation Hash Function Family";
    return tabulation_hash_function_name;
}

int TwistedTabulation::get_number_of_random_bits() {
    return _twister.get_number_of_random_bits() + _simple_tabulation.get_number_of_random_bits();
}

void TwistedTabulation::initialize(int domain_word_size, int range_word_size) {
    HashFunctionFamily::initialize(domain_word_size, range_word_size);
    int number_of_hash_tables = ceil(domain_word_size/range_word_size);

    int tables_domain_size = ceil(domain_word_size/number_of_hash_tables);
    _twister.initialize(domain_word_size-tables_domain_size, tables_domain_size);
    _simple_tabulation.initialize(domain_word_size, tables_domain_size);
}

void TwistedTabulation::build() {
    _twister.build();
    _simple_tabulation.build();
}

int TwistedTabulation::get_word_length() {
    auto word_length = _simple_tabulation.get_word_length();
    return word_length;
}

void TwistedTabulation::evaluate_all_domain() {
//    TODO consider evaluate all domains of twister and simple tabulation instead
//    _twister.evaluate_all_domain();
//    _simple_tabulation.evaluate_all_domain();

    vector<int> evaluated_domain;
    for (int i = 0; i < get_domain_size(); ++i) {
        auto evaluated_value_binary = evaluate(i);
        auto evaluated_value = VectorUtils::binary_to_number(evaluated_value_binary);
        evaluated_domain.push_back(evaluated_value);
    }
    set_evaluated_domain(evaluated_domain);
}

VectorXi TwistedTabulation::evaluate(int value) {
    vector<long> binary_number = VectorUtils::number_to_std_vector(value, get_range_word_length());

    auto binary_head_begin = binary_number.begin();
    auto binary_head_end = binary_number.begin() + get_word_length();
    auto binary_tail_end = binary_number.end();
    vector<long> binary_head(binary_head_begin, binary_head_end);
    vector<long> binary_tail(binary_head_end, binary_tail_end);
    int tail = (int)VectorUtils::std_vector_to_number(binary_tail);

    VectorXi eigen_t = _twister.evaluate(tail);
    vector<long> t = VectorUtils::eigen_vector_to_std_vector(eigen_t);
    vector<long> twisted_head = VectorUtils::xor_vectors(t, binary_head);

    twisted_head.insert(twisted_head.end(), binary_tail.begin(), binary_tail.end());
    auto binary_simple_tabulation_input = VectorUtils::std_vector_to_number(twisted_head);
    auto result = _simple_tabulation.evaluate(binary_simple_tabulation_input);
    return result;
}

string TwistedTabulation::get_function_name() {
    string twisted_hash_hash_function_name = "Twisted Tabulation Hash Function Family";
    return twisted_hash_hash_function_name;
}

int NisanGenerator::get_number_of_random_bits() {
    int number_of_random_bits;
    for(auto& hash_function: _pair_wise_indpendent_hash_funtions) {
        number_of_random_bits += hash_function.get_number_of_random_bits();
    }

    return number_of_random_bits;
}

void NisanGenerator::initialize(int domain_word_length, int k) {
    auto range_word_length = domain_word_length * (int) pow(2, k);
    HashFunctionFamily::initialize(domain_word_length, range_word_length);
    for (int i = 0; i < k-1; ++i) {
        TrivialHashFunctionsFamily pair_wise;
        pair_wise.initialize(domain_word_length, domain_word_length);
        _pair_wise_indpendent_hash_funtions.push_back(pair_wise);
    }
}

void NisanGenerator::build() {
    for(auto& hash_function: _pair_wise_indpendent_hash_funtions) {
        hash_function.build();
    }
}

vector<long> NisanGenerator::evaluate_value(int value) {
    vector<long> accumulated = VectorUtils::number_to_std_vector(value, get_domain_word_length());

    for(auto& hash_function: _pair_wise_indpendent_hash_funtions) {

        int evaluated = hash_function.evaluate_value(value);
        auto binary_evaluated = VectorUtils::number_to_std_vector(evaluated, get_domain_word_length());
        accumulated.insert(accumulated.end(), binary_evaluated.begin(), binary_evaluated.end());
    }
    return accumulated;
}

void NisanGenerator::evaluate_all_domain() {
    vector<int> evaluated_domain;
    for (int i = 0; i < get_domain_size(); ++i) {
        vector<long> binary_evaluated_value = evaluate_value(i);
        int evaluated_value = (int) VectorUtils::std_vector_to_number(binary_evaluated_value);
        evaluated_domain.push_back(evaluated_value);
    }
    set_evaluated_domain(evaluated_domain);
}

string NisanGenerator::get_function_name() {
    string nisan_generator_hash_function_name = "Nisan Generator";
    return nisan_generator_hash_function_name;
}
