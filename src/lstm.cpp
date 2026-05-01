#include "lstm.h"
#include "tdatabase.h"
#include <torch/torch.h>
#include <cstdint>
#include <stdfloat>
#include <iostream>

int run_lstm(const tdataframe &df,
             const tdataframe &df_test,
             const std::vector<stockrow>::size_type batch,
             const std::vector<stockrow>::size_type buffer,
             const std::vector<stockrow>::size_type gap,
             const std::vector<stockrow>::size_type target,
             const std::int64_t hidden_size,
             const std::int64_t epochs)
{
	if (df.sr.size() < batch + gap + target) {
		std::cout << "Too few days to train LSTM\n At least "
		          << batch + gap + target
		          << " days are needed\n";
		return 1;
	}
	if (df_test.sr.size() < df.sr.size()) {
		std::cout << "Test Batch cannot be smaller than training\n";
		return 1;
	}
	if (batch == 0) {
		std::cout << "Batch cannot be zero\n";
		return 1;
	}
	if (buffer == 0) {
		std::cout << "Buffer cannot be zero\n";
		return 1;
	}
	if (target == 0) {
		std::cout << "Target cannot be zero\n";
		return 1;
	}

	const std::vector<stockrow>::size_type batch_n {
		(df.sr.size() - (batch + gap + target - 1)) / buffer
	};
	// static_cast<long>() is there to shut up the narrowing warnings
	torch::Tensor inputx      {
		torch::empty({static_cast<long>(batch_n),
		              static_cast<long>(batch),
		              1                          },
		             torch::TensorOptions().dtype(torch::kFloat))
	};
	torch::Tensor inputy      {
		torch::empty({static_cast<long>(batch_n)},
		             torch::TensorOptions().dtype(torch::kFloat))
	};
	torch::Tensor inputx_test {
		torch::empty({static_cast<long>(batch_n),
		              static_cast<long>(batch),
		              1                               },
		             torch::TensorOptions().dtype(torch::kFloat))
	};
	torch::Tensor inputy_test {
		torch::empty({static_cast<long>(batch_n)},
		             torch::TensorOptions().dtype(torch::kFloat))
	};
	torch::Tensor output;
	torch::Tensor loss;

	df_to_tensor(df,      batch_n, batch, buffer,
	             gap, target, inputx,       inputy    );
	df_to_tensor(df_test, batch_n, batch, buffer,
	             gap, target, inputx_test, inputy_test);

	lstmmodel model {1, hidden_size, 1, static_cast<std::int64_t>(batch_n)};

	torch::optim::Adam optimizer(
		model.parameters(),
		torch::optim::AdamOptions(0.001)
	);
	std::cout << df.ticker
	          << "\nbatch:       " << batch
	          << "\nbuffer:      " << buffer
	          << "\ngap:         " << gap
	          << "\ntarget:      " << target
	          << "\nhidden_size: " << hidden_size
	          << "\nepochs:      " << epochs
	          << "\nbatch_n:     " << batch_n << '\n';
	for (int64_t i {}; i < epochs; ++i) {
		model.train();
		output = model.forward(inputx);
		loss = torch::mse_loss(output, inputy);
		std::cout << std::setw(4) << i << ": " << loss << '\n';
		optimizer.zero_grad();
		loss.backward();
		optimizer.step();
	}
	output = model.forward(inputx_test);
	std::cout  << "\nThe last MSE was:\n"
	          << loss
	          << "\nThe test MSE is:\n"
	          << torch::mse_loss(output, inputy_test) << '\n';

	return 0;
}

int run_lstm_q(const tdataframe &df,
               const tdataframe &df_test,
               const std::vector<stockrow>::size_type batch,
               const std::vector<stockrow>::size_type buffer,
               const std::vector<stockrow>::size_type gap,
               const std::vector<stockrow>::size_type target,
               const std::int64_t hidden_size,
               const std::int64_t epochs)
{
	if (df.sr.size() < batch + gap + target) {
		std::cout << "Too few days to train LSTM\n At least "
		          << batch + gap + target
		          << " days are needed\n";
		return 1;
	}
	if (df_test.sr.size() < df.sr.size()) {
		std::cout << "Test Batch cannot be smaller than training\n";
		return 1;
	}
	if (batch == 0) {
		std::cout << "Batch cannot be zero\n";
		return 1;
	}
	if (buffer == 0) {
		std::cout << "Buffer cannot be zero\n";
		return 1;
	}
	if (target == 0) {
		std::cout << "Target cannot be zero\n";
		return 1;
	}

	const std::vector<stockrow>::size_type batch_n {
		(df.sr.size() - (batch + gap + target - 1)) / buffer
	};
	// static_cast<long>() is there to shut up the narrowing warnings
	torch::Tensor inputx      {
		torch::empty({static_cast<long>(batch_n),
		              static_cast<long>(batch),
		              1                          },
		             torch::TensorOptions().dtype(torch::kFloat))
	};
	torch::Tensor inputy      {
		torch::empty({static_cast<long>(batch_n)},
		             torch::TensorOptions().dtype(torch::kFloat))
	};
	torch::Tensor inputx_test {
		torch::empty({static_cast<long>(batch_n),
		              static_cast<long>(batch),
		              1                               },
		             torch::TensorOptions().dtype(torch::kFloat))
	};
	torch::Tensor inputy_test {
		torch::empty({static_cast<long>(batch_n)},
		             torch::TensorOptions().dtype(torch::kFloat))
	};
	torch::Tensor output;
	torch::Tensor loss;

	df_to_tensor(df,      batch_n, batch, buffer,
	             gap, target, inputx,       inputy    );
	df_to_tensor(df_test, batch_n, batch, buffer,
	             gap, target, inputx_test, inputy_test);

	lstmmodel model {1, hidden_size, 1, static_cast<std::int64_t>(batch_n)};

	torch::optim::Adam optimizer(
		model.parameters(),
		torch::optim::AdamOptions(0.001)
	);
	std::cout << df.ticker
	          << "\nbatch:       " << batch
	          << "\nbuffer:      " << buffer
	          << "\ngap:         " << gap
	          << "\ntarget:      " << target
	          << "\nhidden_size: " << hidden_size
	          << "\nepochs:      " << epochs
	          << "\nbatch_n:     " << batch_n << '\n';
	model.train();
	output = model.forward(inputx);
	loss = torch::mse_loss(output, inputy);
	std::cout << std::setw(4) << 0 << ": " << loss << '\n';
	optimizer.zero_grad();
	loss.backward();
	optimizer.step();
	for (int64_t i {1}; i < epochs; ++i) {
		model.train();
		output = model.forward(inputx);
		loss = torch::mse_loss(output, inputy);
		optimizer.zero_grad();
		loss.backward();
		optimizer.step();
	}
	output = model.forward(inputx_test);
	std::cout  << "\nThe last MSE was:\n"
	          << loss
	          << "\nThe test MSE is:\n"
	          << torch::mse_loss(output, inputy_test) << '\n';

	return 0;
}

void df_to_tensor(const tdataframe &df,
                  const std::vector<stockrow>::size_type batch_n,
                  const std::vector<stockrow>::size_type batch,
                  const std::vector<stockrow>::size_type buffer,
                  const std::vector<stockrow>::size_type gap,
                  const std::vector<stockrow>::size_type target,
                  torch::Tensor &inputx,
                  torch::Tensor &inputy                         )
{
	std::vector<stockrow>::size_type sr_i {};
	std::float16_t target_median {};

	/*
	 * TO DO:
	 * Find a way to fix this static_cast<float>(std::float16_t).
	 */
	for (std::vector<stockrow>::size_type i {}; i < batch_n; ++i) {
		// Training batch
		for (std::vector<stockrow>::size_type batch_i {};
		     batch_i < batch;
		     ++batch_i                                         ) {
			inputx[i][batch_i][0] = static_cast<float>(
				df.sr[sr_i + batch_i].close
			);
			// Uncomment for debug information.
			/*
			std::cout << i << ": "
			          << "train " << batch_i
			          << ": " << df.sr[sr_i + batch_i].close
			          << '\n';
			*/
		}
		// Testing batch
		for (std::vector<stockrow>::size_type test_i {};
		     test_i < target;
		     ++test_i                                         ) {
			target_median += df.sr[sr_i +
			                       test_i +
			                       gap +
			                       batch   ].close;
			// Uncomment for debug information.
			/*
			std::cout << df.sr[sr_i +
			                   test_i +
			                   gap +
			                   batch   ].close
			          << '\n';
			*/
		}
		inputy[i] = static_cast<float>(
			target_median / static_cast<std::float16_t>(target)
		);
		target_median = 0;
		// Uncomment for debug information.
		/*
		std::cout << "--------\n" << inputy[i] << "\n--------\n";
		*/
		sr_i += buffer;
	}

	return;
}