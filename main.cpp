#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <random>
#include <string>
#include <sstream>

int main() {
	using namespace ftxui;
	
	// 游戏状态
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(1, 100);
	int target_number = dis(gen);
	std::string guess_str;      // 输入框绑定的字符串
	int current_guess = 50;
	int attempts = 0;
	std::string message = "🌟 新游戏！猜一个1到100之间的数字！🌟";
	int score = 100;
	
	// 辅助函数：将字符串转换为整数（带错误处理）
	auto parse_guess = [&]() -> bool {
		if (guess_str.empty()) return false;
		try {
			int val = std::stoi(guess_str);
			if (val < 1 || val > 100) return false;
			current_guess = val;
			return true;
		} catch (...) {
			return false;
		}
	};
	
	// 重置游戏
	auto reset_game = [&] {
		target_number = dis(gen);
		guess_str.clear();
		current_guess = 50;
		attempts = 0;
		score = 100;
		message = "🔄 游戏已重置！新的一轮，继续加油！";
	};
	
	// 猜测逻辑
	auto make_guess = [&] {
		if (!parse_guess()) {
			message = "❌ 请输入1~100之间的有效数字！";
			return;
		}
		
		if (attempts == 0) {
			message = "🎯 游戏开始了！";
		}
		attempts++;
		
		if (current_guess == target_number) {
			message = "🎉 恭喜！猜对了！ 你用了 " + std::to_string(attempts) + " 次尝试！";
			message += " 本轮得分：" + std::to_string(score) + "。按[重置]继续挑战！";
			target_number = dis(gen);
			attempts = 0;
			score = 100;
			guess_str.clear();
		} else {
			if (current_guess < target_number) {
				message = "⬆️ 低了！再试试看！";
			} else {
				message = "⬇️ 高了！再试试看！";
			}
			int penalty = std::abs(current_guess - target_number) / 5;
			if (penalty == 0) penalty = 5;
			if (penalty > 50) penalty = 50;
			score -= penalty;
			if (score < 0) score = 0;
			message += " 这次猜测为你减去了" + std::to_string(penalty) + "分。";
		}
		guess_str.clear(); // 清空输入框，方便下次输入
	};
	
	// UI 组件
	auto input_component = Input(&guess_str, "输入一个数字...");
	auto submit_button = Button("提交猜测", make_guess);
	auto reset_button = Button("🔄 重置游戏", reset_game);
	
	// 垂直容器：注意使用 Components{ ... }
	auto container = Container::Vertical(Components{
		input_component,
		submit_button,
		reset_button,
	});
	
	// 渲染器
	auto renderer = Renderer(container, [&] {
		// 使用 Elements 显式构造列表
		Elements children = {
			text("  🎲 猜数字大挑战 🎲  ") | bold | center,
			separator(),
			text("🔢 数字范围: 1 ~ 100") | center,
			text("⭐ 当前积分: " + std::to_string(score) + " / 100") | color(Color::Yellow) | bold,
			text("📝 尝试次数: " + std::to_string(attempts)),
			separator(),
			text(message) | center | border,
			separator(),
			text("🎲 当前猜测: " + std::to_string(current_guess)) | center | bold,
			container->Render() | border,
			separator(),
			text("💡 提示: 在输入框中输入数字，按[提交猜测]或回车") | dim | center,
		};
		return vbox(std::move(children)) | border | size(WIDTH, GREATER_THAN, 60);
	});
	
	auto screen = ScreenInteractive::FitComponent();
	screen.Loop(renderer);
	return 0;
}
