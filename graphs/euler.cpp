// Find euler tour/path of directed graph
// make sure to call reset before playing, and add edges manually
// TODO: return tour with edge indices not nodes
namespace Euler {
	const int MAXN = 1 << 21;
	vector<int> g[MAXN], v[MAXN], gg[MAXN], vv[MAXN];
	vector<bool> deleted;
	vector<int> odds, evens, isolated;
	int n, m;
	void reset(int N) {
		n = N;
		m = 0;
		deleted.clear();
		for (int i = 0; i < n; i++) {
			g[i].clear(), gg[i].clear();
			v[i].clear(), vv[i].clear();
		}
		odds.clear(); evens.clear(); isolated.clear();
	}
	void addEdge(int x, int y, bool oneBased = false) {
		if (oneBased) x--, y--;
		m++;
		g[x].push_back(y);
		v[x].push_back(deleted.size());
		g[y].push_back(x);
		v[y].push_back(deleted.size());
		deleted.push_back(false);
	}
	bool isConnected() {
		vector<bool> vis(n, false);
		int src = -1;
		if (!odds.empty()) src = odds[0];
		if (!evens.empty()) src = evens[0];
		if (src == -1) return true;
		queue<int> q; q.push(src); vis[src] = true;
		int cnt = 0;
		while (!q.empty()) {
			cnt += 1;
			int node = q.front(); q.pop();
			for (int x : g[node])
				if (!vis[x])
					vis[x] = true, q.push(x);
		}
		return cnt == n - int(isolated.size());
	}

	void process(bool force = false) {
		if (evens.size() + odds.size() + isolated.size() > 0 && !force) return;
		evens.clear(); odds.clear(); isolated.clear();
		for (int node = 0; node < n; node++)
			if (g[node].size() == 0) isolated.push_back(node);
			else if (g[node].size() % 2 == 0) evens.push_back(node);
			else odds.push_back(node);
	}

	bool hasEulerCycle() {
		process();
		return odds.empty() && isConnected();
	}
	bool hasEulerPath() {
		process();
		return odds.size() == 2 && isConnected();
	}

	bool getEulerCycle(vector<int> &circuit, int startNode = -1) {
		process();
		circuit.clear();
		if (!odds.empty()) return false;
		if (evens.empty()) return true;
		for (int i = 0; i < n; i++)
			gg[i] = g[i], vv[i] = v[i];
		for (int i = 0; i < m; i++)
			deleted[i] = false;
		int src = startNode != -1 ? startNode : evens[0];
		stack<int> st; st.push(src);
		while (!st.empty()) {
			int node = st.top();
			while (!vv[node].empty() && deleted[vv[node].back()])
				vv[node].pop_back(), gg[node].pop_back();
			if (gg[node].empty()) {
				circuit.push_back(node);
				st.pop();
			}
			else {
				int child = gg[node].back();
				deleted[vv[node].back()] = true;
				st.push(child);
			}
		}
		return int(circuit.size()) == m + 1;
	}

	bool getEulerPath(vector<int> &path, int startNode = -1) {
		process();
		path.clear();
		if (odds.size() != 2) return false;
		if (odds.empty()) return true;
		for (int i = 0; i < n; i++)
			gg[i] = g[i], vv[i] = v[i];
		for (int i = 0; i < m; i++)
			deleted[i] = false;
		if (startNode != odds[0] && startNode != odds[1] && startNode != -1) return false;
		int src = startNode != -1 ? startNode : odds[0];
		stack<int> st; st.push(src);
		while (!st.empty()) {
			int node = st.top();
			while (!vv[node].empty() && deleted[vv[node].back()])
				vv[node].pop_back(), gg[node].pop_back();
			if (gg[node].empty()) {
				path.push_back(node);
				st.pop();
			}
			else {
				int child = gg[node].back();
				deleted[vv[node].back()] = true;
				st.push(child);
			}
		}
		return int(path.size()) == m + 1;
	}
};
