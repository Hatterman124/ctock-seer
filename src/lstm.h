#ifndef LSTM_H
#define LSTM_H

#include "struc.h"
#include <torch/torch.h>
#include <cstdint>
#include <stdfloat>

// The lstm object. This object is called to train.
struct lstmmodel : torch::nn::Module {
	// Constructor
	lstmmodel(int64_t input_size, int64_t hidden_size,
	          int64_t num_layers, int64_t output_size) {
		lstm = torch::nn::LSTM(
			torch::nn::LSTMOptions(input_size, hidden_size)
			           .num_layers(num_layers)
			           .batch_first(true)
			           .bidirectional(true)
		);
		fc   = register_module("fc",
		                       torch::nn::Linear(hidden_size * 2,
		                                         output_size));
	}
	// Function
	torch::Tensor forward(torch::Tensor in) {
		out = std::get<0>(lstm(in));
		out = fc(out.index({torch::indexing::Slice(),
		                    -1,
		                    torch::indexing::Slice()}));
		return out;
	}
	// Data
	torch::nn::LSTM   lstm {nullptr};
	torch::nn::Linear fc   {nullptr};
	torch::Tensor     out;
};

// Converts custom-made dataframe to a tensor object
void df_to_tensors(const dataframe &df,
                   const std::vector<stockrow_b>::size_type buffer,
                   const std::vector<stockrow_b>::size_type batch_n,
                   const std::vector<stockrow_b>::size_type gap,
                   const std::vector<stockrow_b>::size_type target,
                   torch::Tensor &inputx,
                   torch::Tensor &inputy);

// Includes data fom three other tickers (stocks)
void df_to_tensors(const dataframe &df,
                   const std::vector<stockrow_b>::size_type buffer,
                   const std::vector<stockrow_b>::size_type batch_n,
                   const std::vector<stockrow_b>::size_type gap,
                   const std::vector<stockrow_b>::size_type target,
                   const dataframe &dfs0,
                   const dataframe &dfs1,
                   const dataframe &dfs2,
                   torch::Tensor &inputx,
                   torch::Tensor &inputy);

// Trains LSTM and prints results
void run_lstm(const dataframe &df,
              const dataframe &df_test,
              const std::vector<stockrow_b>::size_type buffer,
              const std::vector<stockrow_b>::size_type gap,
              const std::vector<stockrow_b>::size_type target,
              const int64_t hidden_size,
              const int64_t epochs);

// Includes data fom three other tickers (stocks)
void run_lstm(const dataframe &df,
              const dataframe &df_test,
              const std::vector<stockrow_b>::size_type buffer,
              const std::vector<stockrow_b>::size_type gap,
              const std::vector<stockrow_b>::size_type target,
              const int64_t hidden_size,
              const int64_t epochs,
              const dataframe &dfs0,
              const dataframe &dfs1,
              const dataframe &dfs2);

#endif