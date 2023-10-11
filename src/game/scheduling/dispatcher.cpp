/**
 * Canary - A free and open-source MMORPG server emulator
 * Copyright (©) 2019-2022 OpenTibiaBR <opentibiabr@outlook.com>
 * Repository: https://github.com/opentibiabr/canary
 * License: https://github.com/opentibiabr/canary/blob/main/LICENSE
 * Contributors: https://github.com/opentibiabr/canary/graphs/contributors
 * Website: https://docs.opentibiabr.com/
 */

#include "pch.hpp"

#include "game/scheduling/dispatcher.hpp"
#include "lib/thread/thread_pool.hpp"
#include "lib/di/container.hpp"

Dispatcher &Dispatcher::getInstance() {
	return inject<Dispatcher>();
}

void Dispatcher::init() {
	Task::TIME_NOW = std::chrono::system_clock::now();

	threadPool.addLoad([this] {
		std::unique_lock lock(mutex);
		while (!threadPool.getIoContext().stopped()) {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));

			Task::TIME_NOW = std::chrono::system_clock::now();

			size_t sizeTaks = eventTasks.size();
			for (uint_fast64_t i = 0; i < sizeTaks; ++i) {
				auto &task = eventTasks[i];
				if (task.execute()) {
					++dispatcherCycle;
				}
			}

			if (sizeTaks == eventTasks.size()) {
				eventTasks.clear();
			} else {
				eventTasks.erase(eventTasks.begin(), eventTasks.begin() + sizeTaks);
			}

			for (uint_fast64_t i = 0, max = scheduledtasks.size(); i < max && !scheduledtasks.empty(); ++i) {
				const auto &task = scheduledtasks.top();
				if (task->getTime() > Task::TIME_NOW) {
					waitFor(task);
					break;
				}

				task->execute();

				if (!task->isCanceled() && task->isCycle()) {
					scheduledtasks.emplace(task);
				} else {
					scheduledtasksRef.erase(task->getEventId());
				}

				scheduledtasks.pop();
			}

			for (auto &thread : threads) {
				if (!thread.tasks.empty()) {
					eventTasks.insert(eventTasks.end(), make_move_iterator(thread.tasks.begin()), make_move_iterator(thread.tasks.end()));
					thread.tasks.clear();
				}

				if (!thread.scheduledtasks.empty()) {
					for (auto &task : thread.scheduledtasks) {
						scheduledtasks.emplace(task);
						scheduledtasksRef.emplace(task->getEventId(), task);
					}

					eventTasks.insert(eventTasks.end(), make_move_iterator(thread.tasks.begin()), make_move_iterator(thread.tasks.end()));
					thread.scheduledtasks.clear();
				}
			}
		}
	});
}

void Dispatcher::addEvent(std::function<void(void)> &&f, std::string &&context, uint32_t expiresAfterMs) {
	threads[getThreadId()].tasks.emplace_back(expiresAfterMs, f, context);
}

uint64_t Dispatcher::scheduleEvent(const std::shared_ptr<Task> &task) {
	threads[getThreadId()].scheduledtasks.emplace_back(task);
	return scheduledtasksRef.emplace(task->generateId(), task).first->first;
}

uint64_t Dispatcher::scheduleEvent(uint32_t delay, std::function<void(void)> &&f, std::string &&context, bool cycle) {
	const auto &task = std::make_shared<Task>(std::move(f), std::move(context), delay, cycle);
	return scheduleEvent(task);
}

void Dispatcher::stopEvent(uint64_t eventId) {
	auto it = scheduledtasksRef.find(eventId);
	if (it == scheduledtasksRef.end()) {
		return;
	}

	it->second->cancel();
	scheduledtasksRef.erase(it);
}
