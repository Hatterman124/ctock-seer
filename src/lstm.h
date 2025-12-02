#include "struc.h"
#include <torch/torch.h>
#include <cstdint>
#include <stdfloat>
#include <tuple>
#include <iostream>
#include <iomanip>

// PLEASE WORK, PLEASEeeeTEsadey

struct lstmmodel : torch::nn::Module {
	torch::nn::LSTM   lstm {nullptr};
	torch::nn::Linear fc   {nullptr};
	torch::Tensor     out;
	lstmmodel(int64_t input_size, int64_t hidden_size,
	          int64_t num_layers, int64_t output_size) {
		lstm = torch::nn::LSTM(
			torch::nn::LSTMOptions(input_size,
hidden_size).num_layers(num_layers).batch_first(true)
		);
		fc   = register_module("fc",
		                       torch::nn::Linear(hidden_size,
		                                         output_size));
	}
	torch::Tensor forward(torch::Tensor in) {
		out = std::get<0>(lstm(in));
		out = fc(out.index({torch::indexing::Slice(),
		                    -1,
		                    torch::indexing::Slice()}));
		return out;
	}
};

void run_lstm (const dataframe& df) {
	const int64_t epochs {2000};
	std::vector<stockrow_b>::size_type batch_n {df.sr_up.size() / 11};
	size_t ii {};
	torch::Tensor inputx {torch::empty({static_cast<long>(batch_n), 10, 5})};
	torch::Tensor inputy {torch::empty({static_cast<long>(batch_n)})};
	torch::Tensor output;
	torch::Tensor loss;

	for (std::vector<stockrow_b>::size_type i {};
	     i < batch_n; ++i) {

		inputx[i][0][0] = df.sr[ii + 1].close;
		inputx[i][0][1] = df.sr_dif[ii].close;
		inputx[i][0][2] = df.sr_per[ii].close;
		inputx[i][0][3] = df.sr_log[ii].close;
		inputx[i][0][4] = static_cast<double>(df.sr_up[ii].close);
		++ii;
		inputx[i][1][0] = df.sr[ii + 1].close;
		inputx[i][1][1] = df.sr_dif[ii].close;
		inputx[i][1][2] = df.sr_per[ii].close;
		inputx[i][1][3] = df.sr_log[ii].close;
		inputx[i][1][4] = static_cast<double>(df.sr_up[ii].close);
		++ii;
		inputx[i][2][0] = df.sr[ii + 1].close;
		inputx[i][2][1] = df.sr_dif[ii].close;
		inputx[i][2][2] = df.sr_per[ii].close;
		inputx[i][2][3] = df.sr_log[ii].close;
		inputx[i][2][4] = static_cast<double>(df.sr_up[ii].close);
		++ii;
		inputx[i][3][0] = df.sr[ii + 1].close;
		inputx[i][3][1] = df.sr_dif[ii].close;
		inputx[i][3][2] = df.sr_per[ii].close;
		inputx[i][3][3] = df.sr_log[ii].close;
		inputx[i][3][4] = static_cast<double>(df.sr_up[ii].close);
		++ii;
		inputx[i][4][0] = df.sr[ii + 1].close;
		inputx[i][4][1] = df.sr_dif[ii].close;
		inputx[i][4][2] = df.sr_per[ii].close;
		inputx[i][4][3] = df.sr_log[ii].close;
		inputx[i][4][4] = static_cast<double>(df.sr_up[ii].close);
		++ii;
		inputx[i][5][0] = df.sr[ii + 1].close;
		inputx[i][5][1] = df.sr_dif[ii].close;
		inputx[i][5][2] = df.sr_per[ii].close;
		inputx[i][5][3] = df.sr_log[ii].close;
		inputx[i][5][4] = static_cast<double>(df.sr_up[ii].close);
		++ii;
		inputx[i][6][0] = df.sr[ii + 1].close;
		inputx[i][6][1] = df.sr_dif[ii].close;
		inputx[i][6][2] = df.sr_per[ii].close;
		inputx[i][6][3] = df.sr_log[ii].close;
		inputx[i][6][4] = static_cast<double>(df.sr_up[ii].close);
		++ii;
		inputx[i][7][0] = df.sr[ii + 1].close;
		inputx[i][7][1] = df.sr_dif[ii].close;
		inputx[i][7][2] = df.sr_per[ii].close;
		inputx[i][7][3] = df.sr_log[ii].close;
		inputx[i][7][4] = static_cast<double>(df.sr_up[ii].close);
		++ii;
		inputx[i][8][0] = df.sr[ii + 1].close;
		inputx[i][8][1] = df.sr_dif[ii].close;
		inputx[i][8][2] = df.sr_per[ii].close;
		inputx[i][8][3] = df.sr_log[ii].close;
		inputx[i][8][4] = static_cast<double>(df.sr_up[ii].close);
		++ii;
		inputx[i][9][0] = df.sr[ii + 1].close;
		inputx[i][9][1] = df.sr_dif[ii].close;
		inputx[i][9][2] = df.sr_per[ii].close;
		inputx[i][9][3] = df.sr_log[ii].close;
		inputx[i][9][4] = static_cast<double>(df.sr_up[ii].close);
		++ii;
		inputy[i] = df.sr[ii + 1].close;
		++ii;
	}
	lstmmodel model(5, 700, 1, batch_n);
	torch::optim::Adam optimizer(
		model.parameters(),
		torch::optim::AdamOptions(0.001)
	);
	// TRAIN
	for (int64_t i {}; i < epochs; ++i) {
		model.train();
		output = model.forward(inputx);
		loss = torch::mse_loss(output, inputy);
		std::cout << std::setw(4) << i << ": " << loss << '\n';
		optimizer.zero_grad();
		loss.backward();
		optimizer.step();
	}
	std::cout << model.forward(inputx) << '\n'
	          << torch::mse_loss(output, inputy) << '\n';

	return;
}

void run_lstm (const dataframe& df,
               const dataframe& dfs0,
               const dataframe& dfs1,
               const dataframe& dfs2) {
	const int64_t epochs {2000};
	std::vector<stockrow_b>::size_type batch_n {df.sr_up.size() / 11};
	size_t ii {};
	torch::Tensor inputx {torch::empty({static_cast<long>(batch_n), 10, 7})};
	torch::Tensor inputy {torch::empty({static_cast<long>(batch_n)})};
	torch::Tensor output;
	torch::Tensor loss;

	for (std::vector<stockrow_b>::size_type i {};
	     i < batch_n; ++i) {

		inputx[i][0][0] = df.sr[ii + 1].close;
		inputx[i][0][1] = df.sr_dif[ii].close;
		inputx[i][0][2] = df.sr_per[ii].close;
		inputx[i][0][3] = df.sr_log[ii].close;
		inputx[i][0][4] = static_cast<double>(df.sr_up[ii].close);
		inputx[i][0][5] = static_cast<unsigned>(
			df.sr_up[ii].date.month()
		);
		inputx[i][0][6] = dfs0.sr[ii + 1].close *
		                  dfs1.sr[ii + 1].close *
		                  dfs2.sr[ii + 1].close / 3;
		++ii;
		inputx[i][1][0] = df.sr[ii + 1].close;
		inputx[i][1][1] = df.sr_dif[ii].close;
		inputx[i][1][2] = df.sr_per[ii].close;
		inputx[i][1][3] = df.sr_log[ii].close;
		inputx[i][1][4] = static_cast<double>(df.sr_up[ii].close);
		inputx[i][1][5] = static_cast<unsigned>(
			df.sr_up[ii].date.month()
		);
		inputx[i][1][6] = dfs0.sr[ii + 1].close *
		                  dfs1.sr[ii + 1].close *
		                  dfs2.sr[ii + 1].close / 3;
		++ii;
		inputx[i][2][0] = df.sr[ii + 1].close;
		inputx[i][2][1] = df.sr_dif[ii].close;
		inputx[i][2][2] = df.sr_per[ii].close;
		inputx[i][2][3] = df.sr_log[ii].close;
		inputx[i][2][4] = static_cast<double>(df.sr_up[ii].close);
		inputx[i][2][5] = static_cast<unsigned>(
			df.sr_up[ii].date.month()
		);
		inputx[i][2][6] = dfs0.sr[ii + 1].close *
		                  dfs1.sr[ii + 1].close *
		                  dfs2.sr[ii + 1].close / 3;
		++ii;
		inputx[i][3][0] = df.sr[ii + 1].close;
		inputx[i][3][1] = df.sr_dif[ii].close;
		inputx[i][3][2] = df.sr_per[ii].close;
		inputx[i][3][3] = df.sr_log[ii].close;
		inputx[i][3][4] = static_cast<double>(df.sr_up[ii].close);
		inputx[i][3][5] = static_cast<unsigned>(
			df.sr_up[ii].date.month()
		);
		inputx[i][3][6] = dfs0.sr[ii + 1].close *
		                  dfs1.sr[ii + 1].close *
		                  dfs2.sr[ii + 1].close / 3;
		++ii;
		inputx[i][4][0] = df.sr[ii + 1].close;
		inputx[i][4][1] = df.sr_dif[ii].close;
		inputx[i][4][2] = df.sr_per[ii].close;
		inputx[i][4][3] = df.sr_log[ii].close;
		inputx[i][4][4] = static_cast<double>(df.sr_up[ii].close);
		inputx[i][4][5] = static_cast<unsigned>(
			df.sr_up[ii].date.month()
		);
		inputx[i][4][6] = dfs0.sr[ii + 1].close *
		                  dfs1.sr[ii + 1].close *
		                  dfs2.sr[ii + 1].close / 3;
		++ii;
		inputx[i][5][0] = df.sr[ii + 1].close;
		inputx[i][5][1] = df.sr_dif[ii].close;
		inputx[i][5][2] = df.sr_per[ii].close;
		inputx[i][5][3] = df.sr_log[ii].close;
		inputx[i][5][4] = static_cast<double>(df.sr_up[ii].close);
		inputx[i][5][5] = static_cast<unsigned>(
			df.sr_up[ii].date.month()
		);
		inputx[i][5][6] = dfs0.sr[ii + 1].close *
		                  dfs1.sr[ii + 1].close *
		                  dfs2.sr[ii + 1].close / 3;
		++ii;
		inputx[i][6][0] = df.sr[ii + 1].close;
		inputx[i][6][1] = df.sr_dif[ii].close;
		inputx[i][6][2] = df.sr_per[ii].close;
		inputx[i][6][3] = df.sr_log[ii].close;
		inputx[i][6][4] = static_cast<double>(df.sr_up[ii].close);
		inputx[i][6][5] = static_cast<unsigned>(
			df.sr_up[ii].date.month()
		);
		inputx[i][6][6] = dfs0.sr[ii + 1].close *
		                  dfs1.sr[ii + 1].close *
		                  dfs2.sr[ii + 1].close / 3;
		++ii;
		inputx[i][7][0] = df.sr[ii + 1].close;
		inputx[i][7][1] = df.sr_dif[ii].close;
		inputx[i][7][2] = df.sr_per[ii].close;
		inputx[i][7][3] = df.sr_log[ii].close;
		inputx[i][7][4] = static_cast<double>(df.sr_up[ii].close);
		inputx[i][7][5] = static_cast<unsigned>(
			df.sr_up[ii].date.month()
		);
		inputx[i][7][6] = dfs0.sr[ii + 1].close *
		                  dfs1.sr[ii + 1].close *
		                  dfs2.sr[ii + 1].close / 3;
		++ii;
		inputx[i][8][0] = df.sr[ii + 1].close;
		inputx[i][8][1] = df.sr_dif[ii].close;
		inputx[i][8][2] = df.sr_per[ii].close;
		inputx[i][8][3] = df.sr_log[ii].close;
		inputx[i][8][4] = static_cast<double>(df.sr_up[ii].close);
		inputx[i][8][5] = static_cast<unsigned>(
			df.sr_up[ii].date.month()
		);
		inputx[i][8][6] = dfs0.sr[ii + 1].close *
		                  dfs1.sr[ii + 1].close *
		                  dfs2.sr[ii + 1].close / 3;
		++ii;
		inputx[i][9][0] = df.sr[ii + 1].close;
		inputx[i][9][1] = df.sr_dif[ii].close;
		inputx[i][9][2] = df.sr_per[ii].close;
		inputx[i][9][3] = df.sr_log[ii].close;
		inputx[i][9][4] = static_cast<double>(df.sr_up[ii].close);
		inputx[i][9][5] = static_cast<unsigned>(
			df.sr_up[ii].date.month()
		);
		inputx[i][9][6] = dfs0.sr[ii + 1].close *
		                  dfs1.sr[ii + 1].close *
		                  dfs2.sr[ii + 1].close / 3;
		++ii;
		inputy[i] = df.sr[ii + 1].close;
		++ii;
	}
	lstmmodel model(7, 700, 1, batch_n);
	torch::optim::Adam optimizer(
		model.parameters(),
		torch::optim::AdamOptions(0.001)
	);
	// TRAIN
	for (int64_t i {}; i < epochs; ++i) {
		model.train();
		output = model.forward(inputx);
		loss = torch::mse_loss(output, inputy);
		std::cout << std::setw(4) << i << ": " << loss << '\n';
		optimizer.zero_grad();
		loss.backward();
		optimizer.step();
	}
	std::cout << model.forward(inputx) << '\n'
	          << torch::mse_loss(output, inputy) << '\n';

	return;
}



