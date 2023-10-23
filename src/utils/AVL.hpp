#include <memory>
#include <algorithm>
#include <functional>
#include <random>
#include <iostream>

template <typename T, typename Comparator = std::less<T>>
class AVLTree {
private:
	struct Node {
		T value;
		int height;
		std::unique_ptr<Node> left;
		std::unique_ptr<Node> right;

		explicit Node(const T& value) : value(value), height(1), left(nullptr), right(nullptr) {}
	};

	std::unique_ptr<Node> root;
	Comparator compare {};

	inline int getHeight(const std::unique_ptr<Node>& node) const {
		return node ? node->height : 0;
	}

	inline void updateHeight(std::unique_ptr<Node>& node) {
		node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
	}

	std::unique_ptr<Node> rotateLeft(std::unique_ptr<Node>& root) {
		auto newRoot = std::move(root->right);
		root->right = std::move(newRoot->left);
		newRoot->left = std::move(root);

		updateHeight(newRoot->left);
		updateHeight(newRoot);

		return newRoot;
	}

	std::unique_ptr<Node> rotateRight(std::unique_ptr<Node>& root) {
		auto newRoot = std::move(root->left);
		root->left = std::move(newRoot->right);
		newRoot->right = std::move(root);

		updateHeight(newRoot->right);
		updateHeight(newRoot);

		return newRoot;
	}

	std::unique_ptr<Node> balance(std::unique_ptr<Node>& root) {
		int balanceFactor = getHeight(root->left) - getHeight(root->right);

		if (balanceFactor > 1) {
			if (getHeight(root->left->left) >= getHeight(root->left->right)) {
				return rotateRight(root);
			} else {
				root->left = rotateLeft(root->left);
				return rotateRight(root);
			}
		}

		if (balanceFactor < -1) {
			if (getHeight(root->right->right) >= getHeight(root->right->left)) {
				return rotateLeft(root);
			} else {
				root->right = rotateRight(root->right);
				return rotateLeft(root);
			}
		}

		return std::move(root);
	}

	std::unique_ptr<Node> insert(std::unique_ptr<Node>& root, const T& value, bool allowDuplicates) {
		if (!root) return std::make_unique<Node>(value);

        if (compare(value, root->value) || (allowDuplicates && !compare(root->value, value))) {
			root->left = insert(root->left, value, allowDuplicates);
		} else if (compare(root->value, value)) {
			root->right = insert(root->right, value, allowDuplicates);
		}

		updateHeight(root);
		return balance(root);
	}

	std::unique_ptr<Node> removeNode(std::unique_ptr<Node>& root, const T& value) {
		if (!root) return nullptr;

		if (compare(value, root->value)) {
			root->left = removeNode(root->left, value);
		} else if (compare(root->value, value)) {
			root->right = removeNode(root->right, value);
		} else {
			if (!root->left) {
				return std::move(root->right);
			} else if (!root->right) {
				return std::move(root->left);
			}

			Node* maxNode = root->left.get();
			while (maxNode->right) {
				maxNode = maxNode->right.get();
			}
			root->value = maxNode->value;
			root->left = removeNode(root->left, maxNode->value);
		}

		updateHeight(root);
		return balance(root);
	}

	inline bool search(const std::unique_ptr<Node>& root, const T& value) const {
		if (!root) return false;

        if (compare(value, root->value)) {
			return search(root->left, value);
		} else if (compare(root->value, value)) {
			return search(root->right, value);
		} else {
			return true;
		}
	}

	template <typename U>
	std::shared_ptr<U> searchObject(const std::unique_ptr<Node>& root, const T& value) const {
		if (!root) return nullptr;

		if (compare(value, root->value)) {
			return searchObject<U>(root->left, value);
		} else if (compare(root->value, value)) {
			return searchObject<U>(root->right, value);
		} else {
			return std::dynamic_pointer_cast<U>(root->value);
		}
	}

public:
	AVLTree(Comparator comp = Comparator()) : compare(comp) {}

	void clearAll() {
		root.reset();
	}

	void insert(const T& value, bool allowDuplicates = false) {
		root = insert(root, value, allowDuplicates);
	}

	void remove(const T& value) {
		root = removeNode(root, value);
	}

	bool search(const T& value) const {
		return search(root, value);
	}

	template <typename U>
	std::shared_ptr<U> searchObject(const T& value) const {
		return searchObject<U>(root, value);
	}

	static void benchmarkInsert(AVLTree<T>& tree, int NUM_OPERATIONS) {
		std::default_random_engine generator;
		std::uniform_int_distribution<T> distribution(1, 1000000);

		Benchmark bm;
		for (int i = 0; i < NUM_OPERATIONS; ++i) {
			T value = distribution(generator);
			tree.insert(value);
		}
		g_logger().info("Tempo para inserção AVL: - {}ms", bm.duration());
	}

	static void benchmarkInsert1(AVLTree<T>& tree) {
		std::default_random_engine generator;
		std::uniform_int_distribution<T> distribution(1, 1000000);

		Benchmark bm;
		T value = distribution(generator);
		tree.insert(value);
		g_logger().info("Tempo para 1 inserção AVL: - {}ms", bm.duration());
	}

	static void benchmarkSearch(AVLTree<T>& tree, int NUM_OPERATIONS) {
		std::default_random_engine generator;
		std::uniform_int_distribution<T> distribution(1, 1000000);

		Benchmark bm;
		for (int i = 0; i < NUM_OPERATIONS; ++i) {
			T value = distribution(generator);
			tree.search(value);
		}
		g_logger().info("Tempo para busca AVL: - {}ms", bm.duration());
	}

	static void benchmarkSearch1(AVLTree<T>& tree) {
		std::default_random_engine generator;
		std::uniform_int_distribution<T> distribution(1, 1000000);

		Benchmark bm;
		T value = distribution(generator);
		tree.search(value);
		g_logger().info("Tempo para 1 busca AVL: - {}ms", bm.duration());
	}

	static void benchmarkRemove(AVLTree<T>& tree, int NUM_OPERATIONS) {
		std::default_random_engine generator;
		std::uniform_int_distribution<T> distribution(1, 1000000);

		Benchmark bm;
		for (int i = 0; i < NUM_OPERATIONS; ++i) {
			T value = distribution(generator);
			tree.remove(value);
		}
		g_logger().info("Tempo para remoção AVL: - {}ms", bm.duration());
	}

	static void benchmarkClearAll(AVLTree<T>& tree) {
		Benchmark bm;
		tree.clearAll();
		g_logger().info("Tempo para clearAll AVL: - {}ms", bm.duration());
	}

	static void benchmarkAVLTree(AVLTree<T>& tree) {
		const int NUM_OPERATIONS = 1000000;

		benchmarkInsert(tree, NUM_OPERATIONS);
		benchmarkSearch(tree, NUM_OPERATIONS);
		benchmarkRemove(tree, NUM_OPERATIONS);
		benchmarkInsert1(tree);
		benchmarkSearch1(tree);
		benchmarkClearAll(tree);

		g_logger().info("adiciono tudo novamente para medir o ClearAll");
		// adiciono tudo novamente para medir o clearALL
		benchmarkInsert(tree, NUM_OPERATIONS);
		benchmarkClearAll(tree);
	}
};
