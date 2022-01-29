import java.util.*;

public class BetterBST<T extends Comparable<? super T>> extends BinarySearchTree<T> {
	private static final int ELEMENT_WIDTH = 5;

	public BetterBST() {
		this.root = null;
	}

	public BetterBST(BinaryNode<T> root) {
		this.root = root;
	}

	public int height() {
		return height(this.root);
	}

	private int height(BinaryNode<T> root) {
		if (root == null)
			return -1;

		return Math.max(height(root.left), height(root.right)) + 1;
	}

	public T imbalance() {
		return imbalance(this.root);
	}

	private T imbalance(BinaryNode<T> root) {
		if (root == null)
			return null;

		if (Math.abs(height(root.left) - height(root.right)) > 1)
			return root.data;

		T left_imbalance = imbalance(root.left);
		T right_imbalance = imbalance(root.right);

		if (left_imbalance == null)
			return right_imbalance;

		if (right_imbalance == null)
			return left_imbalance;

		return left_imbalance; // could return right_imbalance as well
	}

	public BinarySearchTree<T> mirror() {
		return new BetterBST<T>(mirror(this.root));
	}

	private BinaryNode<T> mirror(BinaryNode<T> root) {
		if (root == null)
			return root;

		return new BinaryNode<T>(root.data, mirror(root.right), mirror(root.left));
	}

	public T smallestGreaterThan(T t) {
		return smallestGreaterThan(t, root);
	}

	private T smallestGreaterThan(T t, BinaryNode<T> root) {
		if (root == null)
			return null;
		else if (t.compareTo(root.data) >= 0)
			return smallestGreaterThan(t, root.right);
		else {
			T left_result = smallestGreaterThan(t, root.left);
			if (left_result == null)
				return root.data;

			return left_result;
		}
	}

	private String padElement(T t) {
		String s = t.toString();
		int leftPad = (ELEMENT_WIDTH - s.length()) / 2;
		int rightPad = ELEMENT_WIDTH - leftPad - s.length();
		for (int i = 0; i < leftPad; i++)
			s = " " + s;
		for (int i = 0; i < rightPad; i++)
			s += " ";
		return s;
	}

	public void prettyPrint() {
		double maxWidth = Math.pow(2, height() + 1) - 1;
		double depth = 0;
		String blank = new String(new char[ELEMENT_WIDTH]).replace('\0', ' ');
		LinkedList<BinaryNode<T>> q = new LinkedList<>();
		LinkedList<BinaryNode<T>> qNext = new LinkedList<>();
		q.offer(root);
		while (depth <= height()) {
			String rowStr = "";
			String rowSpacer = "";
			double spacerWidth = Math.pow(2, height() - depth + 1) - 1;
			double rowWidth = Math.pow(2, height() + 1) - spacerWidth;
			double sidePadding = (maxWidth - rowWidth) / 2;

			for (int i = 0; i < spacerWidth; i++)
				rowSpacer += blank;
			for (int i = 0; i < sidePadding; i++)
				rowStr += blank;

			BinaryNode<T> tmp = null;
			while (q.size() > 0) {
				tmp = q.poll();
				if (tmp != null) {
					rowStr += padElement(tmp.data);
					qNext.offer(tmp.left);
					qNext.offer(tmp.right);
				} else {
					rowStr += blank;
					qNext.offer(null);
					qNext.offer(null);
				}
				rowStr += rowSpacer;
			}

			System.out.println(rowStr);
			q = qNext;
			qNext = new LinkedList<>();
			depth++;
		}
	}

	public static void main(String[] args) {
		BetterBST<Integer> x = new BetterBST<>();
		x.insert(100);
		x.insert(105);
		x.insert(97);
		x.insert(9);
		x.insert(917);
		x.insert(297);
		x.insert(7);
		x.prettyPrint();
	}

}
