#ifndef LSTM_H
#define LSTM_H

#include "tdatabase.h"
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

// Trains LSTM and prints results
int run_lstm(const tdataframe &df,
             const tdataframe &df_test,
             const std::vector<stockrow>::size_type batch,
             const std::vector<stockrow>::size_type buffer,
             const std::vector<stockrow>::size_type gap,
             const std::vector<stockrow>::size_type target,
             const std::int64_t hidden_size,
             const std::int64_t epochs);

int run_lstm_q(const tdataframe &df,
               const tdataframe &df_test,
               const std::vector<stockrow>::size_type batch,
               const std::vector<stockrow>::size_type buffer,
               const std::vector<stockrow>::size_type gap,
               const std::vector<stockrow>::size_type target,
               const std::int64_t hidden_size,
               const std::int64_t epochs);

void df_to_tensor(const tdataframe &df,
                  const std::vector<stockrow>::size_type batch_n,
                  const std::vector<stockrow>::size_type batch,
                  const std::vector<stockrow>::size_type buffer,
                  const std::vector<stockrow>::size_type gap,
                  const std::vector<stockrow>::size_type target,
                  torch::Tensor &inputx,
                  torch::Tensor &inputy                         );

#endif