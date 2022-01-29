import java.util.*;

public class ExpressionTree {

	ExpressionNode root;

	public ExpressionTree() {
		root = null;
	}

	public ExpressionTree(String postfix) throws Exception {
		LinkedList<ExpressionNode> stack = new LinkedList<>();
		String[] postfix_split = postfix.split("\\s+");

		for (int i = 0; i < postfix_split.length; i++) {
			String element = postfix_split[i];
			ExpressionNode temp = new ExpressionNode(element);

			if (temp.isOperator) {
				if (stack.size() < 2) {
					throw new Exception("Stack Underflow: Invalid postfix expression.");
				}

				temp.right = stack.pop();
				temp.left = stack.pop();
			}

			stack.push(temp);
		}

		root = stack.pop();

		if (!stack.isEmpty()) {
			throw new Exception("Non-Empty Stack. Too many operands in the postfix expression.");
		}
	}

	public int eval() {
		return eval(this.root);
	}

	public String postfix() {
		return postfix(this.root);
	}

	public String prefix() {
		return prefix(this.root);
	}

	public String infix() {
		String temp = infix(this.root);
		return temp.substring(1, temp.length() - 1);
	}

	private int eval(ExpressionNode root) {
		if (!root.isOperator)
			return Integer.parseInt(root.data);

		if (root.data.equals("+"))
			return eval(root.left) + eval(root.right);

		else if (root.data.equals("-"))
			return eval(root.left) - eval(root.right);

		else if (root.data.equals("*"))
			return eval(root.left) * eval(root.right);

		else
			return eval(root.left) / eval(root.right);
	}

	private String postfix(ExpressionNode root) {
		if (root == null)
			return "";

		if (!root.isOperator)
			return "" + root.data;

		return postfix(root.left) + " " + postfix(root.right) + " " + root.data;
	}

	private String prefix(ExpressionNode root) {
		if (root == null)
			return "";

		if (!root.isOperator)
			return "" + root.data;

		return root.data + " " + prefix(root.left) + " " + prefix(root.right);
	}

	private String infix(ExpressionNode root) {
		if (root == null)
			return "";

		if (!root.isOperator)
			return root.data;

		return "(" + infix(root.left) + " " + root.data + " " + infix(root.right) + ")";
	}

	private static class ExpressionNode {
		public String data;
		public ExpressionNode left;
		public ExpressionNode right;
		public boolean isOperator;

		public ExpressionNode() {
			this.data = null;
			this.left = null;
			this.right = null;
			this.isOperator = false;
		}

		public ExpressionNode(String data) {
			this.data = data;
			this.left = null;
			this.right = null;
			this.isOperator = determine_type(this.data);
		}

		public ExpressionNode(String data, ExpressionNode left, ExpressionNode right) {
			this.data = data;
			this.left = left;
			this.right = right;
			this.isOperator = determine_type(this.data);
		}

		private boolean determine_type(String data) {
			try {
				int temp = Integer.parseInt(data);
				return false;
			}

			catch (Exception e) {
				return true;
			}
		}
	}

	public static void main(String[] args) {
		ExpressionTree tree;

		try {
			tree = new ExpressionTree("34 2 - 5 *");
			System.out.println("Postfix Expression = " + tree.postfix());
			System.out.println("Prefix Expression = " + tree.prefix());
			System.out.println("Infix Expression = " + tree.infix());
			System.out.println("Evaluated Expression = " + tree.eval());
		}

		catch (Exception e) {
			System.out.println(e);
		}

	}
}
